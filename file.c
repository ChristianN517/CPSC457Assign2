#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct
{
    int pid;
    int arrivalTime;
    int timeUntilFirstResponse;
    int burstLength;
    int startTime;
    int finishTime;
    int waitTime;
    int turnaroundTime;
    int responseTime;
    bool requestCompleted;
    int positionIndex;
} processRequest;

typedef struct
{
    processRequest requests[50];
    int actualArrival;
    int totalBurst;
    int actualStart;
    int actualFinish;
    int totalWait;
    int actualTurnaround;
    int actualResponse;

} Process;

processRequest requestArray[2000];
processRequest requestQueue[2000];
int front = -1, rear = -1;


void enqueue(processRequest value) {
    if (rear == 1000 - 1) {
        printf("queue is full\n");
    } else {
        if (front == -1) {
            front = 0; // initialize front when first element is added
        }
        rear++;
        requestQueue[rear] = value; // insert the request into the queue
    }
}

void dequeue() {
    if (front == -1) {
        printf("queue is empty\n");
    } else {
        front++; // move the front pointer to the next item
        if (front > rear) {
            // reset the queue if all elements are dequeued
            front = -1;
            rear = -1;
        }
    }
}



int addToQueue(processRequest queue[], processRequest array[], int currentTime){
    for (int i = 0; i < 1000; i++)
        {
            if (array[i].arrivalTime == currentTime){
                enqueue(array[i]);
            }

        }
}

processRequest getNextFromQueue(processRequest queue[])
{
    // if (queue[front].pid = -1){
    //     printf("Cannot get elements from empty queue\n");
    // }
    // else{
        return queue[front];
    // }
}

int main(int argc, char *argv[])
{
    FILE *inputFile = fopen(argv[1], "r");
    if (inputFile == NULL)
    {
        perror("Error opening file");
        return 1;
    }

    char lineOutput[100];
    int i = 0;

    while (fgets(lineOutput, sizeof(lineOutput), inputFile) && i < 1000)
    {
        // parse the line and store the values into the processRequest array
        sscanf(lineOutput, "%d,%d,%d,%d",
               &requestArray[i].pid,
               &requestArray[i].arrivalTime,
               &requestArray[i].timeUntilFirstResponse,
               &requestArray[i].burstLength);
        i++;
    }

    fclose(inputFile);


    int timeSimulator;
    int remainingBurst;
    processRequest currentRequest;
    timeSimulator = 0;
    int completedCount = 0;

    //overall while loop which will execute until all processRequests are in completedRequests array
    while (completedCount <= 1000) {
        processRequest nextProcess;
        nextProcess = getNextFromQueue(requestQueue); // func which gets next eligible request from requestQueue
        int currentBurst = nextProcess.burstLength;

        for (int i = 0; i < currentBurst; i++)//loop over until burst increment is complete
        {
            if (i = 0) {
                nextProcess.startTime = timeSimulator;
                nextProcess.responseTime = timeSimulator + nextProcess.timeUntilFirstResponse;
            }

            addToQueue(requestQueue, requestArray, timeSimulator); // will add a process once it's arrival time is matched by timeSimulator
            

            if (currentBurst - 1 <= 0){ //check if burst is completed this iteration
                nextProcess.requestCompleted = true; //if so,  set request completed as true and dequeue
                completedCount += 1; // add to the completedRequests array using request index
                nextProcess.finishTime = timeSimulator;
                nextProcess.waitTime = nextProcess.startTime - nextProcess.arrivalTime;
                nextProcess.turnaroundTime = nextProcess.finishTime - nextProcess.arrivalTime;
                dequeue();
                timeSimulator += 1;

                break; // break so as to no spend unneccessary time
            }
            else{
                currentBurst -= 1;
                timeSimulator += 1;
            }
        }

    }

    Process processArray[50];
    // initialize process values for comparison
    for (int i = 0; i <= 50; i++)
    {
        processArray[i].actualStart = __INT_MAX__;
        processArray[i].actualArrival = __INT_MAX__;
        processArray[i].actualResponse = __INT_MAX__;
        processArray[i].actualFinish = 0;
    }

    // sort requests into arrays by process to calculate totals per process
    for (int i = 0; i < 1000; i++)
    {
        int requestPID = requestArray[i].pid;

        // find empty index in requests
        int n = 0;
        while (n <= 50)
        { // check for each index of array until spot is found to place request
            if (processArray[requestPID].requests[n].pid != requestPID)
            {
                processArray[requestPID].requests[n] = requestArray[i];
                break;
            }
            else
            {
                n += 1;
            }
        }

        // Comparisons to store overall times

        // Initialize values  with extreme numbers to not get null comparison errors

        // Store lowest start time for the process
        if (requestArray[i].startTime < processArray[requestPID].actualStart)
        {
            processArray[requestPID].actualStart = requestArray[i].startTime;
        }

        // Store lowest start time for the process
        if (requestArray[i].arrivalTime < processArray[requestPID].actualArrival)
        {
            processArray[requestPID].actualArrival = requestArray[i].arrivalTime;
        }

        // store lowest response time for the process
        if (requestArray[i].responseTime < processArray[requestPID].actualResponse)
        {
            processArray[requestPID].actualResponse = requestArray[i].responseTime; // need to subtract arrival time from final value
        }

        // store greatest finish time for the process
        if (requestArray[i].finishTime > processArray[requestPID].actualFinish)
        {
            processArray[requestPID].actualFinish = requestArray[i].finishTime;
        }
    }

    // calculating sums, averages and total turnaround
    float averageWaitingTime;
    float averageTurnaroundTime;
    float averageResponseTime;

    for (int i = 0; i <= 50; i++)
    {
        processArray[i].actualTurnaround = processArray[i].actualFinish - processArray[i].actualArrival; // turnaround time calculation

        for (int j = 0; j < 100; j++)
        {
            processArray[i].totalBurst += processArray[i].requests[j].burstLength;
            processArray[i].totalWait += processArray[i].requests[j].waitTime;
        }
        averageWaitingTime += processArray[i].totalWait;
        averageTurnaroundTime += processArray[i].actualTurnaround;
        averageResponseTime += processArray[i].actualResponse;
    }

    averageWaitingTime = averageWaitingTime / 1000;
    averageTurnaroundTime = averageTurnaroundTime / 50;
    averageResponseTime = averageResponseTime / 50;

    printf("Average Waiting Time: %f ms\t", averageWaitingTime);
    printf("Average Turnaround Time: %f ms\t", averageTurnaroundTime);
    printf("Average Response Time: %f ms", averageResponseTime);

    printf("\nSummary:\nPID\tStart\tFinish\tWait\tTurnaround\tResponse\n");
    int len = sizeof(processArray[i].requests) / sizeof(processArray[i].requests[0]);
    for (int i = 1; i <= len; i++)
    {
        printf("%d\t%d\t%d\t%d\t%d\t\t%d\n",
               processArray[i].requests[i % 10].pid,
               processArray[i].actualStart,
               processArray[i].actualFinish,
               processArray[i].totalWait,
               processArray[i].actualTurnaround,
               processArray[i].actualResponse);
    }
}
// Start Time (startTime): The time when the request starts executing.
// Finish Time (finishTime): The time when the request finishes executing.
// Wait Time (waitTime): The time spent waiting in the queue before execution starts (startTime - arrivalTime).

// Turnaround time – amount of time to execute a
// particular process (completion time – arrival time)

// Waiting time – amount of time a process has
// been waiting in the ready queue

// Response time – amount of time it takes from
// when a request was submitted until the first
// response is produced, not output (for time-sharing
// environment)
