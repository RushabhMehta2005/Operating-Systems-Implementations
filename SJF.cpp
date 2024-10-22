#include <iostream>
#include <vector>
#include <algorithm>

class Process {
private:
    int id;
    int arrivalTime;
    int burstTime;
    int waitingTime;
    int turnaroundTime;

public:
    Process(int id, int arrivalTime, int burstTime)
        : id(id), arrivalTime(arrivalTime), burstTime(burstTime), waitingTime(0), turnaroundTime(0) {}

    int getId() const { return id; }
    int getArrivalTime() const { return arrivalTime; }
    int getBurstTime() const { return burstTime; }
    int getWaitingTime() const { return waitingTime; }
    int getTurnaroundTime() const { return turnaroundTime; }

    void setWaitingTime(int waiting) { waitingTime = waiting; }
    void setTurnaroundTime(int turnaround) { turnaroundTime = turnaround; }
};

class SJFScheduler {
private:
    std::vector<Process> processList;
    std::vector<bool> processCompleted;

    void calculateWaitingAndTurnaroundTimes() {
        int currentTime = 0;
        int completedProcesses = 0;
        processCompleted.resize(processList.size(), false);

        while (completedProcesses < processList.size()) {
            std::vector<int> readyQueue;

            // Add all processes that have arrived to the ready queue and are not completed
            for (size_t i = 0; i < processList.size(); ++i) {
                if (processList[i].getArrivalTime() <= currentTime && !processCompleted[i]) {
                    readyQueue.push_back(i);  // Store index of the process
                }
            }

            if (readyQueue.empty()) {
                currentTime++;  // No process has arrived, increment time
                continue;
            }

            // Sort the readyQueue by burst time (Shortest Job First)
            std::sort(readyQueue.begin(), readyQueue.end(), [this](int a, int b) {
                return processList[a].getBurstTime() < processList[b].getBurstTime();
            });

            // Select the process with the shortest burst time
            int selectedIndex = readyQueue.front();
            Process& currentProcess = processList[selectedIndex];

            // Calculate waiting time and turnaround time
            if (currentTime < currentProcess.getArrivalTime()) {
                currentTime = currentProcess.getArrivalTime();
            }
            currentProcess.setWaitingTime(currentTime - currentProcess.getArrivalTime());
            currentProcess.setTurnaroundTime(currentProcess.getWaitingTime() + currentProcess.getBurstTime());

            currentTime += currentProcess.getBurstTime();  // Advance time by the burst time of the current process
            processCompleted[selectedIndex] = true;  // Mark process as completed
            completedProcesses++;  // Increment completed process count
        }
    }

public:
    void addProcess(const Process& process) {
        processList.push_back(process);
    }

    void run() {
        calculateWaitingAndTurnaroundTimes();
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
    SJFScheduler scheduler;

    // Add processes: (id, arrival time, burst time)
    scheduler.addProcess(Process(1, 0, 5));
    scheduler.addProcess(Process(2, 2, 3));
    scheduler.addProcess(Process(3, 4, 1));
    scheduler.addProcess(Process(4, 6, 7));

    scheduler.run();
    scheduler.displayResults();

    return 0;
}
