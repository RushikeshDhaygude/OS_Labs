#include <stdio.h>
#include <limits.h>

struct Process {
    int pid;
    int burst_time;
    int remaining_time;
    int arrival_time;
};

void swap(struct Process *xp, struct Process *yp) {
    struct Process temp = *xp;
    *xp = *yp;
    *yp = temp;
}

void sortProcessesByArrivalTime(struct Process processes[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (processes[j].arrival_time > processes[j + 1].arrival_time) {
                swap(&processes[j], &processes[j + 1]);
            }
        }
    }
}

void printGanttChart(struct Process processes[], int n, int completion_time[]) {
    int total_time = completion_time[n - 1];
    int chart[total_time];
    int current_time = 0;

    for (int i = 0; i < total_time; i++) {
        chart[i] = -1; // Initialize the chart with -1 to represent idle time
    }

    for (int i = 0; i < n; i++) {
        int start_time = completion_time[i] - processes[i].burst_time;
        for (int j = start_time; j < completion_time[i]; j++) {
            chart[j] = processes[i].pid;
        }
    }

    printf("Gantt Chart:\n");
    for (int i = 0; i < total_time; i++) {
        if (i == 0 || chart[i] != chart[i - 1]) {
            if (i != 0) {
                printf("] ");
            }
            printf("| P%d [", chart[i]);
        }
    }
    printf("] |\n");
}

void SJFNonPreemptive(struct Process processes[], int n) {
    sortProcessesByArrivalTime(processes, n);

    int completion_time[n];
    int turnaround_time[n];
    int waiting_time[n];

    float total_waiting_time = 0;
    float total_turnaround_time = 0;

    int current_time = processes[0].arrival_time;
    completion_time[0] = current_time + processes[0].burst_time;
    turnaround_time[0] = completion_time[0] - processes[0].arrival_time;
    waiting_time[0] = 0;

    for (int i = 1; i < n; i++) {
        current_time = completion_time[i - 1];
        completion_time[i] = current_time + processes[i].burst_time;
        turnaround_time[i] = completion_time[i] - processes[i].arrival_time;
        waiting_time[i] = turnaround_time[i] - processes[i].burst_time;

        total_waiting_time += waiting_time[i];
        total_turnaround_time += turnaround_time[i];
    }

    printf("\nNon-Preemptive Shortest Job First Scheduling:\n");
    printf("PID\tBurst Time\tArrival Time\tCompletion Time\tTurnaround Time\tWaiting Time\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t\t%d\t\t%d\t\t%d\t\t%d\n", processes[i].pid, processes[i].burst_time,
               processes[i].arrival_time, completion_time[i], turnaround_time[i], waiting_time[i]);
    }

    printf("Average Waiting Time: %.2f\n", total_waiting_time / n);
    printf("Average Turnaround Time: %.2f\n", total_turnaround_time / n);

    printGanttChart(processes, n, completion_time);
}

void SJFPreemptive(struct Process processes[], int n) {
    sortProcessesByArrivalTime(processes, n);

    int remaining_processes = n;
    int current_time = 0;
    int prev_process = -1; // To track the previously executed process
    int idle_count = 0;    // Counter for consecutive "Idle" outputs

    printf("\nPreemptive Shortest Job First Scheduling:\nGantt Chart:\n");

    while (remaining_processes > 0) {
        int min_burst_index = -1;
        int min_burst_time = INT_MAX; // Set to a large value initially

        for (int i = 0; i < n; i++) {
            if (processes[i].arrival_time <= current_time && processes[i].remaining_time > 0) {
                if (processes[i].remaining_time < min_burst_time) {
                    min_burst_time = processes[i].remaining_time;
                    min_burst_index = i;
                }
            }
        }

        if (min_burst_index == -1) {
            printf("| Idle ");
            idle_count++;

            if (idle_count >= 5) {
                break; // Break the loop if 5 consecutive "Idle" outputs are reached
            }
            current_time++;
        } else {
            idle_count = 0; // Reset idle_count when a non-idle process is executed

            if (prev_process != min_burst_index) {
                printf("| P%d ", processes[min_burst_index].pid);
                prev_process = min_burst_index;
            }
            processes[min_burst_index].remaining_time--;
            current_time++;

            if (processes[min_burst_index].remaining_time == 0) {
                processes[min_burst_index].remaining_time = -1;
                remaining_processes--;
                prev_process = -1; // Reset prev_process for the next iteration
            }
        }
    }

    printf("|\n");

    SJFNonPreemptive(processes, n);
}

void SRTF(struct Process processes[], int n) {
    sortProcessesByArrivalTime(processes, n);

    int remaining_processes = n;
    int current_time = 0;
    int prev_process = -1; // To track the previously executed process
    int idle_count = 0;    // Counter for consecutive "Idle" outputs

    printf("\nShortest Remaining Time First Scheduling:\nGantt Chart:\n");

    while (remaining_processes > 0) {
        int min_burst_index = -1;
        int min_burst_time = INT_MAX; // Set to a large value initially

        for (int i = 0; i < n; i++) {
            if (processes[i].arrival_time <= current_time && processes[i].remaining_time > 0) {
                if (processes[i].remaining_time < min_burst_time) {
                    min_burst_time = processes[i].remaining_time;
                    min_burst_index = i;
                }
            }
        }

        if (min_burst_index == -1) {
            printf("| Idle ");
            idle_count++;

            if (idle_count >= 5) {
                break; // Break the loop if 5 consecutive "Idle" outputs are reached
            }
            current_time++;
        } else {
            idle_count = 0; // Reset idle_count when a non-idle process is executed

            printf("| P%d ", processes[min_burst_index].pid);
            processes[min_burst_index].remaining_time--;
            current_time++;

            if (processes[min_burst_index].remaining_time == 0) {
                remaining_processes--;
            }
        }
    }

    printf("|\n");

    SJFNonPreemptive(processes, n);
}

void roundRobin(struct Process processes[], int n, int time_quantum) {
    int completion_time[n];
    int turnaround_time[n];
    int waiting_time[n];
    int remaining_processes = n;
    int current_time = 0;

    for (int i = 0; i < n; i++) {
        processes[i].remaining_time = processes[i].burst_time;
    }

    printf("\nRound Robin Scheduling (Time Quantum: %d):\nGantt Chart:\n", time_quantum);
    while (remaining_processes > 0) {
        for (int i = 0; i < n; i++) {
            if (processes[i].remaining_time > 0) {
                int execution_time = (processes[i].remaining_time <= time_quantum) ? processes[i].remaining_time : time_quantum;
                current_time += execution_time;
                processes[i].remaining_time -= execution_time;

                printf("| P%d ", processes[i].pid);

                if (processes[i].remaining_time == 0) {
                    completion_time[i] = current_time;
                    remaining_processes--;
                }
            }
        }
    }

    printf("|\n");

    for (int i = 0; i < n; i++) {
        turnaround_time[i] = completion_time[i] - processes[i].arrival_time;
        waiting_time[i] = turnaround_time[i] - processes[i].burst_time;

        printf("Process %d:\n", processes[i].pid);
        printf("Completion Time: %d\n", completion_time[i]);
        printf("Turnaround Time: %d\n", turnaround_time[i]);
        printf("Waiting Time: %d\n\n", waiting_time[i]);
    }
}

int main() {
    int choice, n, time_quantum;

    printf("Enter the number of processes: ");
    scanf("%d", &n);

    struct Process processes[n];
    for (int i = 0; i < n; i++) {
        processes[i].pid = i + 1;
        printf("Enter arrival time for process %d: ", i + 1);
        scanf("%d", &processes[i].arrival_time);
        printf("Enter burst time for process %d: ", i + 1);
        scanf("%d", &processes[i].burst_time);
    }

    do {
        printf("\nMenu:\n");
        printf("1. Shortest Job First (Non-Preemptive)\n");
        printf("2. Shortest Job First (Preemptive)\n");
        printf("3. Shortest Remaining Time First\n");
        printf("4. Round Robin\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                SJFNonPreemptive(processes, n);
                break;
            case 2:
                SJFPreemptive(processes, n);
                break;
            case 3:
                SRTF(processes, n);
                break;
            case 4:
                printf("Enter time quantum for Round Robin: ");
                scanf("%d", &time_quantum);
                roundRobin(processes, n, time_quantum);
                break;
            case 5:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice. Please enter a valid option.\n");
        }
    } while (choice != 5);

    return 0;
}
