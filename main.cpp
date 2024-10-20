#include <iostream>
#include <vector>
#include <iomanip>
#include <algorithm>

using namespace std;

class Process
{
public:
    int pid;
    int burstTime;
    int waitingTime;
    int turnAroundTime;
    Process(int pid, int burstTime)
        : pid(pid), burstTime(burstTime), waitingTime(0), turnAroundTime(0) {}
};

class ProcessSRTF
{
public:
    int pid;
    int burstTime;
    int remainingTime;
    int arrivalTime;
    ProcessSRTF(int pid, int burstTime, int arrivalTime) : pid(pid), burstTime(burstTime), remainingTime(burstTime), arrivalTime(arrivalTime) {}
};

void nonPreemptiveSJF(vector<Process> processes)
{
    sort(processes.begin(), processes.end(), [](Process a, Process b)
         { return a.burstTime < b.burstTime; });

    int totalTime = 0;

    for (int i = 0; i < processes.size(); i++)
    {
        processes[i].waitingTime = totalTime;
        totalTime += processes[i].burstTime;
        processes[i].turnAroundTime = processes[i].waitingTime + processes[i].burstTime;
    }

    cout << "Non-Preemptive SJF Scheduling" << endl;
    cout << "PID\tBurst Time\tWaiting Time\tTurnaround Time" << endl;

    int sumOfWaitingTime = 0;
    int sumOfTurnaroundTime = 0;

    for (auto x : processes)
    {
        cout << x.pid << "\t" << x.burstTime << "\t\t" << x.waitingTime << "\t\t" << x.turnAroundTime << endl;
        sumOfWaitingTime += x.waitingTime;
        sumOfTurnaroundTime += x.turnAroundTime;
    }

    cout << fixed << setprecision(2);
    cout << "Average Waiting Time: " << (float)(sumOfWaitingTime) / (float)(processes.size()) << endl;
    cout << "Average Turnaround Time: " << (float)(sumOfTurnaroundTime) / (float)(processes.size()) << endl;
}

void preemptiveSJF(vector<ProcessSRTF> processes)
{
    int currentTime = 0;
    int completed = 0;
    sort(processes.begin(), processes.end(), [](ProcessSRTF a, ProcessSRTF b)
         { return a.arrivalTime < b.arrivalTime; });

    int minRemainingTime = INT_MAX;
    int shortestProcessIndex = -1;
    int finishTime = 0;
    bool check = false;

    vector<int> waitingTime(processes.size(), 0);
    vector<int> turnAroundTime(processes.size(), 0);
    while (completed != processes.size())
    {
        for (int i = 0; i < processes.size(); i++)
        {
            if (processes[i].arrivalTime <= currentTime && processes[i].remainingTime < minRemainingTime && processes[i].remainingTime > 0)
            {
                minRemainingTime = processes[i].remainingTime;
                shortestProcessIndex = i;
                check = true;
            }
        }
        if (!check)
        {
            currentTime++;
            continue;
        }

        processes[shortestProcessIndex].remainingTime--;
        minRemainingTime = processes[shortestProcessIndex].remainingTime;

        if (minRemainingTime == 0)
            minRemainingTime = INT_MAX;
        if (processes[shortestProcessIndex].remainingTime == 0)
        {
            completed++;
            check = false;
            finishTime = currentTime + 1;
            waitingTime[shortestProcessIndex] = finishTime - processes[shortestProcessIndex].burstTime - processes[shortestProcessIndex].arrivalTime;
            if (waitingTime[shortestProcessIndex] < 0)
                waitingTime[shortestProcessIndex] = 0;

            turnAroundTime[shortestProcessIndex] = processes[shortestProcessIndex].burstTime + waitingTime[shortestProcessIndex];
        }

        currentTime++;
    }

    cout << "Preemptive SJF (SRTF) Scheduling" << endl;
    cout << "PID\tBurst Time\tArrival Time\tWaiting Time\tTurnaround Time" << endl;

    for (int i = 0; i < processes.size(); i++)
        cout << processes[i].pid << "\t" << processes[i].burstTime << "\t\t" << processes[i].arrivalTime << "\t\t" << waitingTime[i] << "\t\t" << turnAroundTime[i] << endl;

    int sumOfWaitingTime = 0;
    int sumOfTurnaroundTime = 0;

    for (int x : waitingTime)
        sumOfWaitingTime += x;
    for (int x : turnAroundTime)
        sumOfTurnaroundTime += x;

    cout << "Average Waiting Time: " << (float)(sumOfWaitingTime) / (float)(processes.size()) << endl;
    cout << "Average Turnaround Time: " << (float)(sumOfTurnaroundTime) / (float)(processes.size()) << endl;
}

int main()
{
    vector<Process> a = {
        {1, 6},
        {2, 8},
        {3, 7},
        {4, 3}};
    vector<ProcessSRTF> b = {
        {1, 6, 0},
        {2, 8, 1},
        {3, 7, 2},
        {4, 3, 3}};
    
    nonPreemptiveSJF(a);

    cout << "\n============================================\n\n";
    
    preemptiveSJF(b);
}