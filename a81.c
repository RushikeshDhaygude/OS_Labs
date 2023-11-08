#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

// Function to find the index of the closest track to the current head position
int findClosestTrack(int tracks[], int n, int head) {
    int minDist = abs(tracks[0] - head);
    int index = 0;

    for (int i = 1; i < n; i++) {
        int dist = abs(tracks[i] - head);

        if (dist < minDist) {
            minDist = dist;
            index = i;
        }
    }

    return index;
}

// SSTF Disk Scheduling Algorithm
void sstf(int tracks[], int n, int head) {
    int totalSeekTime = 0;

    printf("SSTF Disk Scheduling:\n");

    for (int i = 0; i < n; i++) {
        int closestTrackIndex = findClosestTrack(tracks, n, head);
        int seek = abs(tracks[closestTrackIndex] - head);
        totalSeekTime += seek;

        printf("Move head from %d to %d (Seek %d)\n", head, tracks[closestTrackIndex], seek);
        head = tracks[closestTrackIndex];
        tracks[closestTrackIndex] = -1; // Mark track as visited
    }

    printf("Total seek time: %d\n", totalSeekTime);
}

// SCAN Disk Scheduling Algorithm
void scan(int tracks[], int n, int head) {
	int totalSeekTime = 0;
	int direction=1;
	int min=INT_MAX;
	int ind=0;
	
	for(int i=0;i<n;i++){
	if(tracks[i]>=0 && tracks[i]>=head && direction==1){
		dist=abs(tracks[i]-head)
			if(min>dist){
				min=dist;
				ind=i;
			}	
	}
	seek=track[ind];
	printf("\n MOVE HEAD FROM %d to %d with seek(%d)",head,track[ind],seek);
	head=track[ind];
	track[ind]=-1;
	}
	else{
		
	}
    
}

// C-Look Disk Scheduling Algorithm
void clook(int tracks[], int n, int head) {
    int totalSeekTime = 0;

    printf("C-Look Disk Scheduling:\n");

    while (1) {
        int closestTrackIndex = -1;
        int minDist = INT_MAX;

        for (int i = 0; i < n; i++) {
            if (tracks[i] >= 0 && tracks[i] > head && abs(tracks[i] - head) < minDist) {
                minDist = abs(tracks[i] - head);
                closestTrackIndex = i;
            }
        }

        if (closestTrackIndex == -1) {
            // If no tracks are found in the current direction, wrap around
            closestTrackIndex = 0;

            for (int i = 0; i < n; i++) {
                if (tracks[i] >= 0 && tracks[i] < head && tracks[i] > tracks[closestTrackIndex]) {
                    closestTrackIndex = i;
                }
            }
        }

        int seek = abs(tracks[closestTrackIndex] - head);
        totalSeekTime += seek;

        printf("Move head from %d to %d (Seek %d)\n", head, tracks[closestTrackIndex], seek);
        head = tracks[closestTrackIndex];
        tracks[closestTrackIndex] = -1; // Mark track as visited

        int allVisited = 1;

        for (int i = 0; i < n; i++) {
            if (tracks[i] >= 0) {
                allVisited = 0;
                break;
            }
        }

        if (allVisited) break;
    }

    printf("Total seek time: %d\n", totalSeekTime);
}

int main() {
    int n, head, choice;

    printf("Enter the number of tracks: ");
    scanf("%d", &n);

    int tracks[n];

    printf("Enter the track positions:\n");
    for (int i = 0; i < n; i++) {
        scanf("%d", &tracks[i]);
    }

    printf("Enter the initial head position: ");
    scanf("%d", &head);

    while (1) {
        printf("\nDisk Scheduling Algorithms Menu:\n");
        printf("1. SSTF Disk Scheduling\n");
        printf("2. SCAN Disk Scheduling\n");
        printf("3. C-Look Disk Scheduling\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                sstf(tracks, n, head);
                break;
            case 2:
                scan(tracks, n, head);
                break;
            case 3:
                clook(tracks, n, head);
                break;
            case 4:
                exit(0);
            default:
                printf("Invalid choice. Please select a valid option.\n");
                break;
        }
    }

    return 0;
}

