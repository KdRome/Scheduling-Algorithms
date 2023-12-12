#include "process.h"
#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

const int SWITCH_TIME = 2;

int firstComeFirstServe(Process processArray[], int numJobs) {
    int currentTime = 0;
    int totalSwitch = 0;

    //sort the process array based on arrival time
    std::sort(processArray, processArray + numJobs, [](Process &a, Process &b) {
        return a.getArrivalTime() < b.getArrivalTime();
    });

    // Process each job in the order of their arrival
    for (int i = 0; i < numJobs; i++) {
        Process &currentProcess = processArray[i];

        if (currentTime < currentProcess.getArrivalTime()) {
            currentTime = currentProcess.getArrivalTime();
        }
        
        if (i > 0) {
            currentTime += SWITCH_TIME;
            totalSwitch += SWITCH_TIME;
        }
        currentProcess.setWaitingTime(currentTime - currentProcess.getArrivalTime());
        currentProcess.setStartTime(currentTime);
        currentTime += currentProcess.getBurstTime();
        currentProcess.setFinishTime(currentTime);
        currentProcess.setTurnAroundTime(currentTime - currentProcess.getArrivalTime());
    }
    for (int i = 0; i < numJobs; i++) {
        processArray[i].setTotalSwitchTime(totalSwitch);
    }
    return currentTime;
}

int roundRobin(Process processArray[], int numJobs, int quantum) {
    int currentTime = 0;
    std::queue<int> readyQueue;  // Queue to store indices of processes

    // Put all the process indices into the ready queue
    for (int i = 0; i < numJobs; i++) {
        readyQueue.push(i);
        processArray[i].setStartTime(-1); // Initialize start time as -1 (not started)
    }

    // Process each job in the ready queue
    while (!readyQueue.empty()) {
        int processIndex = readyQueue.front();
        readyQueue.pop();
        Process &currentProcess = processArray[processIndex];  // Reference to the process in the array

        // Update the start time for the process if it's not started yet
        if (currentProcess.getStartTime() == -1) {
            currentProcess.setStartTime(currentTime);
        }

        // Calculate the time the process will execute in this round
        int timeToExecute = std::min(currentProcess.getRemainingBurstTime(), quantum);
        currentProcess.setRemainingBurstTime(currentProcess.getRemainingBurstTime() - timeToExecute);
        currentTime += timeToExecute;

        // Check if the process is finished
        if (currentProcess.getRemainingBurstTime() <= 0) {
            currentProcess.setFinishTime(currentTime);
            currentProcess.setTurnAroundTime(currentTime - currentProcess.getArrivalTime());
            currentProcess.setWaitingTime(currentProcess.getTurnAroundTime() - currentProcess.getBurstTime());
        } else {
            // If the process is not finished, add it back to the queue
            readyQueue.push(processIndex);
        }

        // Update total switch time after each process execution
        if (!readyQueue.empty()) {
            currentTime += SWITCH_TIME;
        }
    }

    // Update the total switch time for each process
    for (int i = 0; i < numJobs; i++) {
        processArray[i].setTotalSwitchTime(currentTime - processArray[i].getArrivalTime() - processArray[i].getBurstTime());
    }

    return currentTime;
}