#include <iostream>
#include <queue>
#include <vector>

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

class FCFS_Scheduler {
private:
    std::vector<Process> processQueue;

    void calculateWaitingAndTurnaroundTimes() {
        int currentTime = 0;
        for (auto& process : processQueue) {
            if (currentTime < process.getArrivalTime()) {
                currentTime = process.getArrivalTime();  // CPU waits for the next process to arrive
            }
            process.setWaitingTime(currentTime - process.getArrivalTime());
            process.setTurnaroundTime(process.getWaitingTime() + process.getBurstTime());
            currentTime += process.getBurstTime();
        }
    }

public:
    void addProcess(const Process& process) {
        processQueue.push_back(process);
    }

    void run() {
        calculateWaitingAndTurnaroundTimes();
    }

    void displayResults() const {
        int totalWaitingTime = 0;
        int totalTurnaroundTime = 0;

        std::cout << "PID\tArrival\tBurst\tWaiting\tTurnaround\n";
        for (const auto& process : processQueue) {
            std::cout << process.getId() << "\t" 
                      << process.getArrivalTime() << "\t"
                      << process.getBurstTime() << "\t"
                      << process.getWaitingTime() << "\t"
                      << process.getTurnaroundTime() << "\n";

            totalWaitingTime += process.getWaitingTime();
            totalTurnaroundTime += process.getTurnaroundTime();
        }

        int n = processQueue.size();
        std::cout << "\nAverage Waiting Time: " << static_cast<float>(totalWaitingTime) / n << "\n";
        std::cout << "Average Turnaround Time: " << static_cast<float>(totalTurnaroundTime) / n << "\n";
    }
};

int main() {
    FCFS_Scheduler scheduler;

    // Add processes: (id, arrival time, burst time)
    scheduler.addProcess(Process(1, 0, 5));
    scheduler.addProcess(Process(2, 2, 3));
    scheduler.addProcess(Process(3, 4, 1));
    scheduler.addProcess(Process(4, 6, 7));

    scheduler.run();
    scheduler.displayResults();

    return 0;
}
