//
//  main.cpp
//  CSC-332-Scheduling
//

#include <iostream>
#include <iomanip>
#include <fstream>
#include "process.h"
#include "algorithms.h"

using namespace std;

const int MAX_JOBS = 100;

void outputData(Process processArray[], int numJobs, int totalSwitchTime);
void switchAlgorithm(int num, Process processArray[], int numJobs, int quantum);

void switchAlgorithm(int num, Process processArray[], int numJobs, int quantum) {
    int endTime;
    
    switch (num) {
        case 0:
            cout << "First Come First Serve (non-preemptive):" << endl << endl;
            endTime = firstComeFirstServe(processArray, numJobs);
            break;
            
        case 1:
            cout << "Round Robin (preemptive): " << endl << endl;
            endTime = roundRobin(processArray, numJobs, quantum);
            break;
            
        default:
            return;
    }
    outputData(processArray, numJobs, endTime);
}

void outputData(Process processArray[], int numJobs, int endTime){
    
    double efficiency = 0.0;
    int avgTAT = 0;
    int avgWaitingTime = 0;
    int totalTAT = 0;
    int totalWaitingTime = 0;
    int totalTime = 0;
    int totalBurstTime = 0;

    for(int i = 0; i < numJobs; i++){
        totalTAT += processArray[i].getTurnAroundTime();
        totalWaitingTime += processArray[i].getWaitingTime();
        totalBurstTime += processArray[i].getBurstTime();
    }
    
    efficiency = ((double)totalBurstTime / (totalBurstTime + processArray[numJobs - 1].getTotalSwitchTime())) * 100;
    totalTime = endTime - processArray[0].getStartTime();
    avgTAT = totalTAT / numJobs;
    avgWaitingTime = totalWaitingTime / numJobs;
    
    cout << "Total Time: " << totalTime << " time units" << endl;
    cout << "Average TAT: " << avgTAT << " time units" << endl;
    cout << "Average Waiting Time: " << avgWaitingTime << " time units" << endl;
    cout << "CPU Efficiency: " << fixed << setprecision(1) << efficiency << "%" << endl << endl;
    cout << left << setw(15) << "Process" << setw(20) << "Service Time" << setw(20) << "Turn Around Time" << endl;

    for(int i = 0; i < numJobs; i++){
        cout << left << setw(15) << ("P" + to_string(processArray[i].getProcessId()))
             << setw(20) << processArray[i].getBurstTime()
             << setw(20) << processArray[i].getTurnAroundTime() << endl;
    }

    cout << endl;
}

int main() {
    const int NUM_ALGORITHMS = 2;
    const int QUANTUM = 50;
    Process processArray[MAX_JOBS];
    int processId, arrivalTime, burstTime, numJobs;
    
    ifstream file("/Users/jawadrada/Desktop/CSC-332-Scheduling/SchedulingAlgorithmsInput.txt");
    if (!file.is_open()) {
        cout << "Error opening file" << endl;
        return -1;
    }
    
    file >> numJobs;
    for (int i = 0; i < numJobs; i++) {
        file >> processId >> arrivalTime >> burstTime;
        processArray[i] = Process(processId, arrivalTime, burstTime);
    }
    
    file.close();
    
    for(int i = 0; i < NUM_ALGORITHMS; i++){
        switchAlgorithm(i, processArray, numJobs, QUANTUM);
    }
    
    return 0;
}
