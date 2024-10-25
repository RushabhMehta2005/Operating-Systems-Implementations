#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>

class Process {
private:
    int id;
    int arrivalTime;
    int burstTime;
    int remainingTime;
    int waitingTime;
    int turnaroundTime;

public:
    Process(int id, int arrivalTime, int burstTime)
        : id(id), arrivalTime(arrivalTime), burstTime(burstTime), remainingTime(burstTime), waitingTime(0), turnaroundTime(0) {}

    int getId() const { return id; }
    int getArrivalTime() const { return arrivalTime; }
    int getBurstTime() const { return burstTime; }
    int getRemainingTime() const { return remainingTime; }
    int getWaitingTime() const { return waitingTime; }
    int getTurnaroundTime() const { return turnaroundTime; }

    void setRemainingTime(int time) { remainingTime = time; }
    void addWaitingTime(int time) { waitingTime += time; }
    void setTurnaroundTime(int time) { turnaroundTime = time; }
};

class RoundRobinScheduler {
private:
    std::vector<Process> processList;
    int timeQuantum;

public:
    RoundRobinScheduler(int tq) : timeQuantum(tq) {}

    void addProcess(const Process& process) {
        processList.push_back(process);
    }

    void run() {
        int currentTime = 0;
        std::queue<int> readyQueue; // Queue stores the index of the process in processList
        std::vector<bool> processCompleted(processList.size(), false);
        std::vector<bool> inQueue(processList.size(), false);
        int completedProcesses = 0;

        // Initially, add processes that have arrived at time 0
        for (size_t i = 0; i < processList.size(); ++i) {
            if (processList[i].getArrivalTime() <= currentTime) {
                readyQueue.push(i);
                inQueue[i] = true;
            }
        }

        while (completedProcesses < processList.size()) {
            if (readyQueue.empty()) {
                // If no process is in the ready queue, move time forward to the next arriving process
                currentTime++;
                for (size_t i = 0; i < processList.size(); ++i) {
                    if (!inQueue[i] && processList[i].getArrivalTime() <= currentTime) {
                        readyQueue.push(i);
                        inQueue[i] = true;
                    }
                }
                continue;
            }

            int processIndex = readyQueue.front();
            readyQueue.pop();

            Process& currentProcess = processList[processIndex];
            inQueue[processIndex] = false;

            int executionTime = std::min(timeQuantum, currentProcess.getRemainingTime());
            currentProcess.setRemainingTime(currentProcess.getRemainingTime() - executionTime);
            currentTime += executionTime;

            // Add waiting time for all processes in the ready queue during this time slice
            for (size_t i = 0; i < processList.size(); ++i) {
                if (i != processIndex && inQueue[i] && processList[i].getArrivalTime() <= currentTime) {
                    processList[i].addWaitingTime(executionTime);
                }
            }

            // If process is not yet completed, requeue it; else, mark it as completed
            if (currentProcess.getRemainingTime() > 0) {
                readyQueue.push(processIndex);
                inQueue[processIndex] = true;
            } else {
                currentProcess.setTurnaroundTime(currentTime - currentProcess.getArrivalTime());
                completedProcesses++;
            }

            // Add new processes arriving during this time slice
            for (size_t i = 0; i < processList.size(); ++i) {
                if (!inQueue[i] && !processCompleted[i] && processList[i].getArrivalTime() <= currentTime) {
                    readyQueue.push(i);
                    inQueue[i] = true;
                }
            }
        }
    }

    void displayResults() const {
        int totalWaitingTime = 0;
        int totalTurnaroundTime = 0;

        std::cout << "PID\tArrival\tBurst\tWaiting\tTurnaround\n";
        for (const auto& process : processList) {
            std::cout << process.getId() << "\t" 
                      << process.getArrivalTime() << "\t"
                      << process.getBurstTime() << "\t"
                      << process.getWaitingTime() << "\t"
                      << process.getTurnaroundTime() << "\n";

            totalWaitingTime += process.getWaitingTime();
            totalTurnaroundTime += process.getTurnaroundTime();
        }

        int n = processList.size();
        std::cout << "\nAverage Waiting Time: " << static_cast<float>(totalWaitingTime) / n << "\n";
        std::cout << "Average Turnaround Time: " << static_cast<float>(totalTurnaroundTime) / n << "\n";
    }
};

int main() {
    int timeQuantum = 2;
    RoundRobinScheduler scheduler(timeQuantum);

    // Add processes: (id, arrival time, burst time)
    scheduler.addProcess(Process(1, 0, 5));
    scheduler.addProcess(Process(2, 1, 3));
    scheduler.addProcess(Process(3, 2, 1));
    scheduler.addProcess(Process(4, 3, 2));
    scheduler.addProcess(Process(5, 4, 4));

    scheduler.run();
    scheduler.displayResults();

    return 0;
}
