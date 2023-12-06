#include "process.h"
#include <iostream>

using namespace std;

const int SWITCH_TIME = 2;

int firstComeFirstServe(Process processArray[], int numJobs) {
    int currentTime = 0;
    int totalSwitch = 0;

    for (int i = 0; i < numJobs; i++) {
        if (currentTime < processArray[i].getArrivalTime()) {
            currentTime = processArray[i].getArrivalTime();
        }
        if (i > 0) {
            currentTime += SWITCH_TIME;
            totalSwitch += SWITCH_TIME;
        }
        processArray[i].setWaitingTime(currentTime - processArray[i].getArrivalTime());
        processArray[i].setStartTime(currentTime);
        currentTime += processArray[i].getBurstTime();
        processArray[i].setFinishTime(currentTime);
        processArray[i].setTurnAroundTime(currentTime - processArray[i].getArrivalTime());
    }
    processArray[numJobs - 1].setTotalSwitchTime(totalSwitch);
    return currentTime;
}

int roundRobin(Process processArray[], int numJobs, int quantum) {
    int currentTime = 0;
    int completedJobs = 0;
    int totalSwitch = 0;
    bool addSwitchTime = false;

    while(completedJobs < numJobs) {
        for(int i = 0; i < numJobs; i++) {
            
            if(processArray[i].getArrivalTime() <= currentTime && processArray[i].getRemainingBurstTime() > 0) {
                if(addSwitchTime) {
                    currentTime += SWITCH_TIME;
                    totalSwitch += SWITCH_TIME;
                }

                if(processArray[i].getRemainingBurstTime() == processArray[i].getBurstTime()) {
                    processArray[i].setStartTime(currentTime);
                    processArray[i].setWaitingTime(currentTime - processArray[i].getArrivalTime());
                }

                int timeSpent = min(quantum, processArray[i].getRemainingBurstTime());
                currentTime += timeSpent;
                processArray[i].setRemainingBurstTime(processArray[i].getRemainingBurstTime() - timeSpent);
                if(processArray[i].getRemainingBurstTime() == 0) {
                    processArray[i].setFinishTime(currentTime);
                    processArray[i].setTurnAroundTime(processArray[i].getFinishTime() - processArray[i].getArrivalTime());
                    completedJobs++;
                }
                addSwitchTime = true;
            }
        }
    }
    
    processArray[numJobs - 1].setTotalSwitchTime(totalSwitch);
    return currentTime;
}
