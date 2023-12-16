#include "process.h"
#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <climits>

using namespace std;

const int SWITCH_TIME = 2;

int firstComeFirstServe(Process processArray[], int numJobs) {
    int currentTime = 0;
    int totalSwitch = 0;
    vector<int> executed;
    vector<int> readyQueue;

    while (executed.size() < numJobs) {
        // Add processes to readyQueue based on arrivalTime
        for (int i = 0; i < numJobs; i++) {
            if (processArray[i].getArrivalTime() <= currentTime 
                && !count(executed.begin(), executed.end(), processArray[i].getProcessId()) 
                || !count(readyQueue.begin(), readyQueue.end(), processArray[i].getProcessId())) {
                readyQueue.push_back(i);
            }
        }
        if (!readyQueue.empty()) {
            Process& currentProcess = processArray[readyQueue.front()];
            readyQueue.erase(readyQueue.begin());

            currentProcess.setWaitingTime(currentTime - currentProcess.getArrivalTime());
            currentProcess.setStartTime(currentTime);
            //if its the first or last process add only one switch time unit
            if (executed.size() == 0 || executed.size() == numJobs - 1) {
                currentTime += (SWITCH_TIME / 2);
                totalSwitch += (SWITCH_TIME / 2);
            }
            else{
                currentTime += SWITCH_TIME;
                totalSwitch += SWITCH_TIME;
            }
            // Execute the process
            currentTime += currentProcess.getBurstTime();
            currentProcess.setFinishTime(currentTime);
            currentProcess.setTurnAroundTime(currentTime - currentProcess.getArrivalTime());
            executed.push_back(currentProcess.getProcessId());
        }
        else {
            // Find the next process that will arrive and jump time
            int nextArrivalTime = INT_MAX;
            for (int i = 0; i < numJobs; i++) {
                if (!count(executed.begin(), executed.end(), processArray[i].getProcessId()) && !count(readyQueue.begin(), readyQueue.end(), processArray[i].getProcessId())) {
                    if (processArray[i].getArrivalTime() < nextArrivalTime) {
                        nextArrivalTime = processArray[i].getArrivalTime();
                    }
                }
            }
            // Update currentTime if a valid next arrival time is found
            if (nextArrivalTime != INT_MAX) {
                currentTime = nextArrivalTime;
            }
        }
    }

    processArray[numJobs - 1].setTotalSwitchTime(totalSwitch);
    return currentTime;
}

int roundRobin(Process processArray[], int numJobs, int quantum) {
    int currentTime = 0;
    int totalSwitch = 0;
    queue<int> readyQueue;
    vector<int> executed;
    
    // Add processes to readyQueue
    for (int i = 0; i < numJobs; i++) {
        readyQueue.push(i);
        processArray[i].setStartTime(-1);
    }
    // Execute processes in readyQueue
    while (!readyQueue.empty()) {
        int processIndex = readyQueue.front();
        readyQueue.pop();
        Process &currentProcess = processArray[processIndex];
        // Set start time if it hasn't been set
        if (currentProcess.getStartTime() == -1) {
            currentProcess.setStartTime(currentTime);
        }
        // Calculate time to execute
        int timeToExecute = min(currentProcess.getRemainingBurstTime(), quantum);
        currentProcess.setRemainingBurstTime(currentProcess.getRemainingBurstTime() - timeToExecute);
        currentTime += timeToExecute;
        // Check if process has finished
        if (currentProcess.getRemainingBurstTime() <= 0) {
            currentProcess.setFinishTime(currentTime);
            currentProcess.setTurnAroundTime(currentTime - currentProcess.getArrivalTime());
            currentProcess.setWaitingTime(currentProcess.getTurnAroundTime() - currentProcess.getBurstTime());
            executed.push_back(currentProcess.getProcessId());
        } else {
            readyQueue.push(processIndex);
        }
        // Update switch time
        if (readyQueue.empty()) {
            currentTime += quantum;
            totalSwitch += quantum;
        } else {
            currentTime += SWITCH_TIME;
            totalSwitch += SWITCH_TIME;
        }
    }
    // Set total switch time
    processArray[numJobs - 1].setTotalSwitchTime(totalSwitch);
    return currentTime;
}
