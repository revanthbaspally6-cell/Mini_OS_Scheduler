#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct Process {
    int pid;
    char name[20];
    int burst;
    int priority;
    int arrival;
    int remaining;
    struct Process *next;
} Process;
typedef struct QNode {
    Process *p;
    struct QNode *next;
} QNode;

typedef struct StackNode {
    Process *p;
    struct StackNode *next;
} StackNode;

typedef struct BST {
    Process *p;
    int key;
    struct BST *left, *right;
} BST;

int quantum = 0;

void enqueue(QNode **head, QNode **tail, Process *p) {
    QNode node = (QNode)malloc(sizeof(QNode));
    node->p = p;
    node->next = NULL;
    if (*tail == NULL) {
        *head = *tail = node;
    } else {
        (*tail)->next = node;
        *tail = node;
    }
}

Process* dequeue(QNode **head, QNode **tail) {
    if (*head == NULL) return NULL;
    QNode *t = *head;
    Process *p = t->p;
    *head = t->next;
    if (*head == NULL) *tail = NULL;
    free(t);
    return p;
}

void pushTerm(StackNode **top, Process *p) {
    StackNode n = (StackNode)malloc(sizeof(StackNode));
    n->p = p;
    n->next = *top;
    *top = n;
}

Process* popTerm(StackNode **top) {
    if (!*top) return NULL;
    StackNode *t = *top;
    Process *p = t->p;
    *top = t->next;
    free(t);
    return p;
}

BST* insertBST(BST *root, Process *p, int key) {
    if (!root) {
        BST node = (BST)malloc(sizeof(BST));
        node->p = p;
        node->key = key;
        node->left = node->right = NULL;
        return node;
    }
    if (key < root->key) root->left = insertBST(root->left, p, key);
    else root->right = insertBST(root->right, p, key);
    return root;
}

void bstToQueue_inorder(BST *root, QNode **head, QNode **tail) {
    if (!root) return;
    bstToQueue_inorder(root->left, head, tail);
    enqueue(head, tail, root->p);
    bstToQueue_inorder(root->right, head, tail);
}

void Execute(Process *p) {
    printf("Executing PID=%d (%s) [burst=%d remaining=%d]\n",
           p->pid, p->name, p->burst, p->remaining);
}

void Terminate(StackNode **top, Process *p) {
    printf("Terminating PID=%d (%s)\n", p->pid, p->name);
    pushTerm(top, p);
}

QNode* processFCFS(BST *root) {
    QNode *head = NULL, *tail = NULL;
    bstToQueue_inorder(root, &head, &tail);
    return head;
}

QNode* processPriority(BST *root) {
    QNode *head = NULL, *tail = NULL;
    bstToQueue_inorder(root, &head, &tail);
    return head;
}

QNode* processSJF(BST *root) {
    QNode *head = NULL, *tail = NULL;
    bstToQueue_inorder(root, &head, &tail);
    return head;
}

QNode* processRoundRobin(BST *root, int tq) {
    QNode *head = NULL, *tail = NULL;
    bstToQueue_inorder(root, &head, &tail);
    return head;
}

void FCFS(Process *head) {
    BST *tree = NULL;
    Process *t = head;
    while (t) {
        tree = insertBST(tree, t, t->arrival);
        t = t->next;
    }
    QNode *ready = processFCFS(tree);
    StackNode *terminated = NULL;
    int time = 0;

    printf("\n--- FCFS Schedule ---\n");
    while (ready) {
        Process p = dequeue(&ready, (QNode*)&ready);
        if (time < p->arrival) time = p->arrival;
        Execute(p);
        printf("Gantt: [%d - %d] PID=%d (%s)\n",
               time, time + p->burst, p->pid, p->name);
        time += p->burst;
        p->remaining = 0;
        Terminate(&terminated, p);
    }
    printf("--- FCFS Complete ---\n");
}

void PriorityScheduling(Process *head) {
    BST *tree = NULL;
    Process *t = head;
    while (t) {
        tree = insertBST(tree, t, t->priority);
        t = t->next;
    }
    QNode *ready = processPriority(tree);
    StackNode *terminated = NULL;
    int time = 0;

    printf("\n--- Priority Schedule ---\n");
    while (ready) {
        Process p = dequeue(&ready, (QNode*)&ready);
        if (time < p->arrival) time = p->arrival;
        Execute(p);
        printf("Gantt: [%d - %d] PID=%d (%s) priority=%d\n",
               time, time + p->burst, p->pid, p->name, p->priority);
        time += p->burst;
        p->remaining = 0;
        Terminate(&terminated, p);
    }
    printf("--- Priority Complete ---\n");
}

void SJF(Process *head) {
    BST *tree = NULL;
    Process *t = head;
    while (t) {
        tree = insertBST(tree, t, t->burst);
        t = t->next;
    }
    QNode *ready = processSJF(tree);
    StackNode *terminated = NULL;
    int time = 0;

    printf("\n--- SJF Schedule ---\n");
    while (ready) {
        Process p = dequeue(&ready, (QNode*)&ready);
        if (time < p->arrival) time = p->arrival;
        Execute(p);
        printf("Gantt: [%d - %d] PID=%d (%s) burst=%d\n",
               time, time + p->burst, p->pid, p->name, p->burst);
        time += p->burst;
        p->remaining = 0;
        Terminate(&terminated, p);
    }
    printf("--- SJF Complete ---\n");
}

void RoundRobin(Process *head, int tq) {
    if (tq <= 0) {
        printf("Enter Time Quantum: ");
        scanf("%d", &tq);
    }
    BST *tree = NULL;
    Process *t = head;
    while (t) {
        tree = insertBST(tree, t, t->arrival);
        t = t->next;
    }

    QNode *ready = processRoundRobin(tree, tq);
    StackNode *terminated = NULL;

    QNode *rq_head = ready, *rq_tail = NULL;
    if (rq_head) {
        rq_tail = rq_head;
        while (rq_tail->next) rq_tail = rq_tail->next;
    }

    int time = 0;
    printf("\n--- Round Robin (quantum=%d) ---\n", tq);

    while (rq_head) {
        Process *p = dequeue(&rq_head, &rq_tail);

        if (time < p->arrival) time = p->arrival;

        Execute(p);

        int slice = p->remaining > tq ? tq : p->remaining;
        printf("Gantt: [%d - %d] PID=%d (%s) ran for %d\n",
               time, time + slice, p->pid, p->name, slice);

        p->remaining -= slice;
        time += slice;

        if (p->remaining <= 0) {
            Terminate(&terminated, p);
        } else {
            enqueue(&rq_head, &rq_tail, p);
        }
    }
    printf("--- Round Robin Complete ---\n");
}

Process* addProcess(Process *head) {
    Process n = (Process)malloc(sizeof(Process));
    printf("\nEnter PID: ");
    scanf("%d", &n->pid);
    printf("Enter Process Name: ");
    scanf("%s", n->name);
    printf("Enter Burst Time: ");
    scanf("%d", &n->burst);
    printf("Enter Priority: ");
    scanf("%d", &n->priority);
    printf("Enter Arrival Time: ");
    scanf("%d", &n->arrival);

    n->remaining = n->burst;
    n->next = NULL;

    if (!head) return n;
    Process *t = head;
    while (t->next) t = t->next;
    t->next = n;
    return head;
}

int main() {
    Process *plist = NULL;
    char ch;
    int choice;

    while (1) {
        printf("\nWant to enter a process? (Y/N): ");
        scanf(" %c", &ch);
        if (ch == 'N' || ch == 'n') break;
        plist = addProcess(plist);
    }

    printf("\nSELECT SCHEDULING ALGORITHM\n");
    printf("1. FCFS\n2. PRIORITY\n3. SJF\n4. ROUND ROBIN\n");
    scanf("%d", &choice);

    switch (choice) {
        case 1: FCFS(plist); break;
        case 2: PriorityScheduling(plist); break;
        case 3: SJF(plist); break;
        case 4: RoundRobin(plist, quantum); break;
        default: printf("Invalid Choice\n");
    }
    return 0;
}
