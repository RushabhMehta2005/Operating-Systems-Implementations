#include <iostream>
#include <vector>
#include <algorithm>

class Process {
private:
    int id;
    int arrivalTime;
    int burstTime;
    int remainingTime;
    int priority;
    int waitingTime;
    int turnaroundTime;
    bool isCompleted;

public:
    Process(int id, int arrivalTime, int burstTime, int priority)
        : id(id), arrivalTime(arrivalTime), burstTime(burstTime), remainingTime(burstTime),
          priority(priority), waitingTime(0), turnaroundTime(0), isCompleted(false) {}

    int getId() const { return id; }
    int getArrivalTime() const { return arrivalTime; }
    int getBurstTime() const { return burstTime; }
    int getRemainingTime() const { return remainingTime; }
    int getPriority() const { return priority; }
    int getWaitingTime() const { return waitingTime; }
    int getTurnaroundTime() const { return turnaroundTime; }
    bool getIsCompleted() const { return isCompleted; }

    void setRemainingTime(int time) { remainingTime = time; }
    void addWaitingTime(int time) { waitingTime += time; }
    void setTurnaroundTime(int time) { turnaroundTime = time; }
    void completeProcess() { isCompleted = true; }
};

class PreemptivePriorityScheduler {
private:
    std::vector<Process> processList;
    int currentTime;

    bool allProcessesCompleted() {
        for (const auto& process : processList) {
            if (!process.getIsCompleted()) {
                return false;
            }
        }
        return true;
    }

    int findNextProcess() {
        int highestPriorityIndex = -1;
        int highestPriority = INT_MAX;

        for (size_t i = 0; i < processList.size(); ++i) {
            if (!processList[i].getIsCompleted() && processList[i].getArrivalTime() <= currentTime) {
                if (processList[i].getPriority() < highestPriority ||
                    (processList[i].getPriority() == highestPriority && processList[i].getRemainingTime() > 0)) {
                    highestPriority = processList[i].getPriority();
                    highestPriorityIndex = i;
                }
            }
        }
        return highestPriorityIndex;
    }

    void updateWaitingTimes(int currentProcess) {
        for (size_t i = 0; i < processList.size(); ++i) {
            if (i != currentProcess && processList[i].getArrivalTime() <= currentTime && !processList[i].getIsCompleted()) {
                processList[i].addWaitingTime(1); // Add 1 unit of time to all waiting processes
            }
        }
    }

public:
    PreemptivePriorityScheduler() : currentTime(0) {}

    void addProcess(const Process& process) {
        processList.push_back(process);
    }

    void run() {
        while (!allProcessesCompleted()) {
            int currentProcessIndex = findNextProcess();

            if (currentProcessIndex == -1) {
                currentTime++;  // If no process is ready, move time forward
                continue;
            }

            Process& currentProcess = processList[currentProcessIndex];

            currentProcess.setRemainingTime(currentProcess.getRemainingTime() - 1);
            currentTime++;

            // Update waiting times for other processes
            updateWaitingTimes(currentProcessIndex);

            if (currentProcess.getRemainingTime() == 0) {
                currentProcess.setTurnaroundTime(currentTime - currentProcess.getArrivalTime());
                currentProcess.completeProcess();
            }
        }
    }

    void displayResults() const {
        int totalWaitingTime = 0;
        int totalTurnaroundTime = 0;

        std::cout << "PID\tArrival\tBurst\tPriority\tWaiting\tTurnaround\n";
        for (const auto& process : processList) {
            std::cout << process.getId() << "\t" 
                      << process.getArrivalTime() << "\t"
                      << process.getBurstTime() << "\t"
                      << process.getPriority() << "\t\t"
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
    PreemptivePriorityScheduler scheduler;

    // Add processes: (id, arrival time, burst time, priority)
    scheduler.addProcess(Process(1, 0, 7, 3));
    scheduler.addProcess(Process(2, 2, 4, 1));
    scheduler.addProcess(Process(3, 4, 1, 4));
    scheduler.addProcess(Process(4, 5, 4, 2));

    scheduler.run();
    scheduler.displayResults();

    return 0;
}
