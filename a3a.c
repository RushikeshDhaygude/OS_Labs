#include <stdio.h>
#include <stdlib.h>

struct Process {
    int id;
    int arrivalTime;
    int burstTime;
    int remainingTime;
    int completionTime;
};

void sortByArrivalTime(struct Process processes[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (processes[j].arrivalTime > processes[j + 1].arrivalTime) {
                struct Process temp = processes[j];
                processes[j] = processes[j + 1];
                processes[j + 1] = temp;
            }
        }
    }
}

int findShortestJob(struct Process processes[], int n, int currentTime) {
    int shortestJobIndex = -1;
    int shortestBurst = 99999;
    for (int i = 0; i < n; i++) {
        if (processes[i].arrivalTime <= currentTime && processes[i].remainingTime > 0 &&
            processes[i].remainingTime < shortestBurst) {
            shortestJobIndex = i;
            shortestBurst = processes[i].remainingTime;
        }
    }
    return shortestJobIndex;
}

void roundRobin() {
    int n, quantum;
    printf("Enter the number of processes: ");
    scanf("%d", &n);
    printf("Enter the time quantum: ");
    scanf("%d", &quantum);

    struct Process processes[n];

    for (int i = 0; i < n; i++) {
        processes[i].id = i + 1;
        printf("Enter arrival time and burst time for process %d: ", processes[i].id);
        scanf("%d %d", &processes[i].arrivalTime, &processes[i].burstTime);
        processes[i].remainingTime = processes[i].burstTime;
    }

    int currentTime = 0;
    int completedProcesses = 0;
    int completionTime[n];
    int turnaroundTime[n];
    int waitingTime[n];

    while (completedProcesses < n) {
        for (int i = 0; i < n; i++) {
            if (processes[i].remainingTime > 0) {
                if (processes[i].remainingTime <= quantum) {
                    currentTime += processes[i].remainingTime;
                    processes[i].remainingTime = 0;
                    completionTime[i] = currentTime;
                    completedProcesses++;
                } else {
                    currentTime += quantum;
                    processes[i].remainingTime -= quantum;
                }
            }
        }
    }

    int totalTurnaroundTime = 0;
    int totalWaitingTime = 0;

    for (int i = 0; i < n; i++) {
        turnaroundTime[i] = completionTime[i] - processes[i].arrivalTime;
        waitingTime[i] = turnaroundTime[i] - processes[i].burstTime;
        totalTurnaroundTime += turnaroundTime[i];
        totalWaitingTime += waitingTime[i];
    }

    printf("\nProcess\tArrival\tBurst\tCompletion\tTurnaround\tWaiting\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t%d\t\t%d\t\t%d\n", processes[i].id, processes[i].arrivalTime,
               processes[i].burstTime, completionTime[i], turnaroundTime[i], waitingTime[i]);
    }

    double averageTurnaroundTime = (double)totalTurnaroundTime / n;
    double averageWaitingTime = (double)totalWaitingTime / n;

    printf("\nAverage Turnaround Time: %.2f\n", averageTurnaroundTime);
    printf("Average Waiting Time: %.2f\n", averageWaitingTime);
}




int sjfp() {
    int n;
    printf("Enter the number of processes: ");
    scanf("%d", &n);
    struct Process processes[n];

    for (int i = 0; i < n; i++) {
        processes[i].id = i + 1;
        printf("Enter arrival time and burst time for process %d: ", processes[i].id);
        scanf("%d %d", &processes[i].arrivalTime, &processes[i].burstTime);
        processes[i].remainingTime = processes[i].burstTime;
    }

    sortByArrivalTime(processes, n);
    int currentTime = 0;
    int completedProcesses = 0;
    int shortestJobIndex = -1;
    int completionTime[n];
    int turnaroundTime[n];
    int waitingTime[n];

    struct GanttEntry {
        struct Process process;
        int startTime;
    };

    struct GanttEntry ganttChart[n * 2];  // Max size (twice the number of processes)

    int ganttIndex = 0;
    ganttChart[ganttIndex].process = processes[0];
    ganttChart[ganttIndex].startTime = 0;
    ganttIndex++;

    while (completedProcesses < n) {
        shortestJobIndex = findShortestJob(processes, n, currentTime);
        if (shortestJobIndex == -1) {
            currentTime++;
        } else {
            processes[shortestJobIndex].remainingTime--;
            currentTime++;
            if (processes[shortestJobIndex].remainingTime == 0) {
                completedProcesses++;
                completionTime[shortestJobIndex] = currentTime;
            }
            if (ganttIndex == 0 || ganttChart[ganttIndex - 1].process.id != processes[shortestJobIndex].id) {
                ganttChart[ganttIndex].process = processes[shortestJobIndex];
                ganttChart[ganttIndex].startTime = currentTime - 1;
                ganttIndex++;
            }
        }
    }

    int totalTurnaroundTime = 0;
    int totalWaitingTime = 0;

    for (int i = 0; i < n; i++) {
        turnaroundTime[i] = completionTime[i] - processes[i].arrivalTime;
        waitingTime[i] = turnaroundTime[i] - processes[i].burstTime;
        totalTurnaroundTime += turnaroundTime[i];
        totalWaitingTime += waitingTime[i];
    }

    printf("\nGantt Chart:\n");
    for (int i = 0; i < ganttIndex; i++) {
        printf("P%d-%d ", ganttChart[i].process.id, ganttChart[i].startTime);
    }
    printf("\n");

    printf("\nProcess\tArrival\tBurst\tCompletion\tTurnaround\tWaiting\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t%d\t\t%d\t\t%d\n", processes[i].id, processes[i].arrivalTime,
               processes[i].burstTime,
               completionTime[i], turnaroundTime[i], waitingTime[i]);
    }

    double averageTurnaroundTime = (double)totalTurnaroundTime / n;
    double averageWaitingTime = (double)totalWaitingTime / n;

    printf("\nAverage Turnaround Time: %.2f\n", averageTurnaroundTime);
    printf("Average Waiting Time: %.2f\n", averageWaitingTime);

    return 0;
}

int main() {
    int choice;
    while (1) {
        printf("Menu:\n");
        printf("1. SJF Preemptive\n");
        printf("2. Round Robin\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                sjfp();
                break;
            case 2:
                roundRobin();
                break; 
            case 3:
                printf("Exiting the program\n");
                return 0;
            default:
                printf("Invalid choice\n");
        }
    }

    return 0;
}

