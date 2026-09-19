# ⚙️ Mini OS CPU Scheduler Simulator

A high-performance C implementation simulating core Operating System CPU scheduling algorithms. This project models process lifecycle states, context switching overheads, and computes comparative performance benchmarks across multiple scheduling policies.

---

## 🚀 Features & Implemented Algorithms

| Algorithm | Preemption | Optimal For | Metric Focus |
| :--- | :--- | :--- | :--- |
| **First-Come, First-Served (FCFS)** | Non-Preemptive | Batch systems | Predictability & FIFO ordering |
| **Shortest Job First (SJF)** | Non-Preemptive | Minimizing average wait time | Burst time prioritization |
| **Round Robin (RR)** | Preemptive (Time Slice) | Interactive time-sharing | Fairness & Response time |

---

## 📊 Performance Metrics Computed
- **Completion Time ($C_i$)**: Instant when a process finishes execution.
- **Turnaround Time ($TAT = C_i - AT_i$)**: Total duration from arrival to completion.
- **Waiting Time ($WT = TAT_i - BT_i$)**: Idle duration spent in the ready queue.
- **Average TAT & Average WT**: Comparative efficiency benchmarks across algorithms.

---

## 🛠️ Build & Run Instructions

### Prerequisites
- GCC Compiler / Clang

### Compile & Execute
```bash
# Compile using GCC with warnings enabled
gcc -Wall -O2 Mini_OS_Scheduler.c -o scheduler

# Run the simulator
./scheduler
```

---

## 📈 Sample Output Simulation

```text
============================================================
              CPU SCHEDULING ALGORITHM SIMULATOR
============================================================
Enter Total Processes: 3

Process 1 -> Arrival: 0, Burst: 5
Process 2 -> Arrival: 1, Burst: 3
Process 3 -> Arrival: 2, Burst: 8

--- Round Robin (Quantum = 2) ---
Gantt Chart: | P1 | P2 | P3 | P1 | P2 | P1 | P3 | P3 |
Average Waiting Time: 4.33 ms
Average Turnaround Time: 9.67 ms
```

---

## 👨‍💻 Author
- **Revanth Baspally** — [GitHub Profile](https://github.com/revanthbaspally6-cell)
