#include <iostream>
#include <vector>
#include <iomanip>
#include <algorithm>

using namespace std;

// Lớp tiến tình cho thuật thuật toán SJF
class Process
{
public:
    int pid;             // ID của tiến trình
    int burstTime;       // Thời gian burst (thời gian thực thi)
    int waitingTime;     // Thời gian chờ
    int turnAroundTime;  // Thời gian quay vòng
    Process(int pid, int burstTime)
        : pid(pid), burstTime(burstTime), waitingTime(0), turnAroundTime(0) {}
};

// Lớp tiến tình cho thuật thuật toán SRTF
class ProcessSRTF
{
public:
    int pid;               // ID của tiến trình
    int burstTime;         // Thời gian burst
    int remainingTime;     // Thời gian còn lại
    int arrivalTime;       // Thời gian đến của tiến trình
    ProcessSRTF(int pid, int burstTime, int arrivalTime)
        : pid(pid), burstTime(burstTime), remainingTime(burstTime), arrivalTime(arrivalTime) {}
};

// Thuật toán SJF
void nonPreemptiveSJF(vector<Process> processes)
{
    // Sắp xếp các tiến trình theo thời gian burst tăng dần
    sort(processes.begin(), processes.end(), [](Process a, Process b)
         { return a.burstTime < b.burstTime; });

    int totalTime = 0; // Biến tổng thời gian cho đến thời điểm hiện tại

    // Tính thời gian chờ và thời gian quay vòng cho từng tiến trình
    for (int i = 0; i < processes.size(); i++)
    {
        processes[i].waitingTime = totalTime;  // Thời gian chờ là tổng thời gian trước đó
        totalTime += processes[i].burstTime;   // Cập nhật tổng thời gian
        processes[i].turnAroundTime = processes[i].waitingTime + processes[i].burstTime;
    }

    // In bảng thông tin tiến trình sau khi lập lịch
    cout << "Non-Preemptive SJF Scheduling" << endl;
    cout << "PID\tBurst Time\tWaiting Time\tTurnaround Time" << endl;

    int sumOfWaitingTime = 0;
    int sumOfTurnaroundTime = 0;

    // Tính tổng thời gian chờ và thời gian quay vòng
    for (auto x : processes)
    {
        cout << x.pid << "\t" << x.burstTime << "\t\t" << x.waitingTime << "\t\t" << x.turnAroundTime << endl;
        sumOfWaitingTime += x.waitingTime;
        sumOfTurnaroundTime += x.turnAroundTime;
    }

    // In thời gian chờ trung bình và thời gian quay vòng trung bình
    cout << fixed << setprecision(2);
    cout << "Average Waiting Time: " << (float)(sumOfWaitingTime) / (float)(processes.size()) << endl;
    cout << "Average Turnaround Time: " << (float)(sumOfTurnaroundTime) / (float)(processes.size()) << endl;
}

// Thuật toán SRTF
void preemptiveSJF(vector<ProcessSRTF> processes)
{
    int currentTime = 0;        // Thời gian hiện tại trong quá trình lập lịch
    int completed = 0;          // Số lượng tiến trình đã hoàn thành

    sort(processes.begin(), processes.end(), [](ProcessSRTF a, ProcessSRTF b)
         { return a.arrivalTime < b.arrivalTime; }); 

    int minRemainingTime = INT_MAX;  // Thời gian còn lại nhỏ nhất
    int shortestProcessIndex = -1;   // Chỉ số của tiến trình có thời gian còn lại nhỏ nhất
    int finishTime = 0;              // Thời gian kết thúc của tiến trình
    bool check = false;              // Biến kiểm tra xem có tiến trình nào sẵn sàng không

    vector<int> waitingTime(processes.size(), 0);     // Danh sách thời gian chờ của các tiến trình
    vector<int> turnAroundTime(processes.size(), 0);  // Danh sách thời gian quay vòng của các tiến trình

    // Lặp cho đến khi tất cả tiến trình hoàn thành
    while (completed != processes.size())
    {
        // Tìm tiến trình có thời gian còn lại nhỏ nhất trong các tiến trình đã đến
        for (int i = 0; i < processes.size(); i++)
        {
            if (processes[i].arrivalTime <= currentTime &&
                processes[i].remainingTime < minRemainingTime &&
                processes[i].remainingTime > 0)
            {
                minRemainingTime = processes[i].remainingTime;
                shortestProcessIndex = i;
                check = true;
            }
        }

        // Nếu không có tiến trình nào đến, tăng thời gian hiện tại
        if (!check)
        {
            currentTime++;
            continue;
        }

        // Giảm thời gian còn lại của tiến trình ngắn nhất
        processes[shortestProcessIndex].remainingTime--;
        minRemainingTime = processes[shortestProcessIndex].remainingTime;

        // Nếu tiến trình hoàn thành, cập nhật số tiến trình đã hoàn thành
        if (minRemainingTime == 0)
            minRemainingTime = INT_MAX;
        if (processes[shortestProcessIndex].remainingTime == 0)
        {
            completed++;  // Tăng số lượng tiến trình đã hoàn thành
            check = false;
            finishTime = currentTime + 1;  // Thời gian kết thúc của tiến trình hiện tại
            waitingTime[shortestProcessIndex] = finishTime - processes[shortestProcessIndex].burstTime - processes[shortestProcessIndex].arrivalTime;

            // Nếu thời gian chờ âm, thiết lập thời gian chờ bằng 0
            if (waitingTime[shortestProcessIndex] < 0)
                waitingTime[shortestProcessIndex] = 0;

            // Tính thời gian quay vòng
            turnAroundTime[shortestProcessIndex] = processes[shortestProcessIndex].burstTime + waitingTime[shortestProcessIndex];
        }

        currentTime++;  // Tăng thời gian hiện tại
    }

    // In bảng thông tin tiến trình sau khi lập lịch
    cout << "Preemptive SJF (SRTF) Scheduling" << endl;
    cout << "PID\tBurst Time\tArrival Time\tWaiting Time\tTurnaround Time" << endl;

    for (int i = 0; i < processes.size(); i++)
        cout << processes[i].pid << "\t" << processes[i].burstTime << "\t\t" << processes[i].arrivalTime << "\t\t" << waitingTime[i] << "\t\t" << turnAroundTime[i] << endl;

    int sumOfWaitingTime = 0;
    int sumOfTurnaroundTime = 0;

    // Tính tổng thời gian chờ và thời gian quay vòng
    for (int x : waitingTime)
        sumOfWaitingTime += x;
    for (int x : turnAroundTime)
        sumOfTurnaroundTime += x;

    // In thời gian chờ trung bình và thời gian quay vòng trung bình
    cout << "Average Waiting Time: " << (float)(sumOfWaitingTime) / (float)(processes.size()) << endl;
    cout << "Average Turnaround Time: " << (float)(sumOfTurnaroundTime) / (float)(processes.size()) << endl;
}

int main()
{
    // Danh sách tiến trình cho thuật toán SJF 
    vector<Process> a = {
        {1, 6},
        {2, 8},
        {3, 7},
        {4, 3}};
    // Danh sách tiến trình cho thuật toán SRTF
    vector<ProcessSRTF> b = {
        {1, 6, 0},
        {2, 8, 1},
        {3, 7, 2},
        {4, 3, 3}};
    
    nonPreemptiveSJF(a);

    cout << "\n============================================\n\n";
    

    preemptiveSJF(b);
}
