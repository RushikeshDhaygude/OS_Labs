#include <stdio.h> 
#include <stdlib.h> 
#include <sys/types.h> 
#include <sys/wait.h> 
#include <unistd.h>

void bubbleSort(int arr[], int n)
{
    int temp, i, j;
    for (i = 0; i < n - 1; i++)
    {
        for (j = 0; j < n - i - 1; j++)
        {
            if (arr[j] > arr[j + 1])
            {
                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

int main()
{
    int n, i;
    printf("Size of array: ");
    scanf("%d", &n);
    int arr[n];

    printf("Enter %d numbers:\n", n);
    for (i = 0; i < n; i++)
    {
        scanf("%d", &arr[i]);
    }

    int ch;
    printf("\nEnter choice:\n");
    printf("1. Fork, Wait, and Merge/Bubble Sort\n");
    printf("2. Orphan Process\n");
    printf("3. Zombie Process\n");
    scanf("%d", &ch);

    switch (ch)
    {
    case 1:
    {
        pid_t pid = fork();

        if (pid < 0)
        {
            printf("Fork failed.\n");
            exit(1);
        }
        else if (pid == 0)
        {
            printf("\nChild process (Bubble Sort) started.\n");
            bubbleSort(arr, n);
            printf("\nSorted array by the child process (Bubble Sort):\n");
            for (i = 0; i < n; i++)

                printf("%d ", arr[i]);
            printf("\n");
        }
        else
        {
            printf("\nParent process (Bubble Sort) started.\n");
             bubbleSort(arr, n);
            printf("\nSorted array by the parent process (Bubble Sort):\n");
            for (i = 0; i < n; i++)
                printf("%d ", arr[i]);
            printf("\n");
            wait(NULL);
            printf("\nChild process has completed.\n");
        }
        break;
    }
    case 2:
    {
        pid_t pid = fork();

        if (pid < 0)
        {
            printf("Fork failed.\n");
            exit(1);
        }
        else if (pid == 0)
        {
            // Orphan process
            printf("\nOrphan process started (PID: %d).\n", getpid());
            printf("Parent process (PID: %d) terminated before the child process.\n",
                   getppid());

            // Sleep to create an orphan process sleep(10);
            sleep(5);

            printf("Orphan process (PID: %d) completed.\n", getpid());
        }
        else
        {
            // Parent process
            printf("\nParent process (PID: %d) started.\n", getppid());

            sleep(5);
            system("ps -elf | grep -e 'PPID\\|CHILD'");

            printf("Parent process (PID: %d) completed.\n", getppid());
        }
        break;
    }
    case 3:
    {
        pid_t pid = fork();

        if (pid < 0)
        {
            printf("Fork failed.\n");
            exit(1);
        }
        else if (pid == 0)
        {
            // Child process
            printf("\nChild process (Zombie) started.\n");

            printf("Child process (PID: %d) completed.\n", getpid());
        }
        else
        {
            // Parent process sleep(10);
            sleep(10);
            char command[100];
            sprintf(command, "ps -elf | grep %d", getpid());
            system(command);
            printf("\nParent process (Zombie) started.\n");
            printf("Parent process will sleep to create a Zombie (PID: %d).\n", pid);
            printf("Parent process (PID: %d) completed.\n", getppid());

            //wait(NULL);
        }
        break;
    }
    default:
        printf("Invalid choice.\n");
        break;
    }

    return 0;
}
