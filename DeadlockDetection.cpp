#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>

class Resource {
private:
    int id;
    int total_instances;
    int available_instances;

public:
    Resource(int resourceId, int totalInstances) 
        : id(resourceId), total_instances(totalInstances), available_instances(totalInstances) {}

    int getId() const { return id; }
    int getTotalInstances() const { return total_instances; }
    int getAvailableInstances() const { return available_instances; }

    void allocate(int instances) { 
        if (instances <= available_instances) {
            available_instances -= instances;
        }
    }

    void release(int instances) {
        if (instances > 0 && available_instances + instances <= total_instances) {
            available_instances += instances;
        }
    }
};

class Process {
private:
    int id;
    std::unordered_map<int, int> allocated_resources;
    std::unordered_map<int, int> requested_resources;

public:
    Process(int processId) : id(processId) {}

    int getId() const { return id; }

    void requestResource(int resourceId, int instances) {
        requested_resources[resourceId] += instances;
    }

    void allocateResource(int resourceId, int instances) {
        allocated_resources[resourceId] += instances;
        requested_resources[resourceId] -= instances;
    }

    void releaseResource(int resourceId, int instances) {
        if (allocated_resources[resourceId] >= instances) {
            allocated_resources[resourceId] -= instances;
        }
    }

    const std::unordered_map<int, int>& getAllocatedResources() const { 
        return allocated_resources; 
    }

    const std::unordered_map<int, int>& getRequestedResources() const { 
        return requested_resources; 
    }
};

class DeadlockDetector {
private:
    std::vector<Resource> resources;
    std::vector<Process> processes;

    bool hasCycle(std::unordered_map<int, std::unordered_set<int>>& graph) {
        std::unordered_set<int> visited;
        std::unordered_set<int> rec_stack;

        for (const auto& process : processes) {
            if (detectCycleDFS(process.getId(), graph, visited, rec_stack)) {
                return true;
            }
        }
        return false;
    }

    bool detectCycleDFS(int processId, 
                        std::unordered_map<int, std::unordered_set<int>>& graph, 
                        std::unordered_set<int>& visited, 
                        std::unordered_set<int>& rec_stack) {
        if (rec_stack.find(processId) != rec_stack.end()) {
            return true;
        }

        if (visited.find(processId) != visited.end()) {
            return false;
        }

        visited.insert(processId);
        rec_stack.insert(processId);

        for (int adjacentProcess : graph[processId]) {
            if (detectCycleDFS(adjacentProcess, graph, visited, rec_stack)) {
                return true;
            }
        }

        rec_stack.erase(processId);
        return false;
    }

public:
    DeadlockDetector(const std::vector<Resource>& resourceList, 
                     const std::vector<Process>& processList)
        : resources(resourceList), processes(processList) {}

    bool detectDeadlock(const std::unordered_map<int, std::unordered_set<int>>& graph) {
        // Create a copy of the graph to avoid modifying the input
        auto graphCopy = graph;
        return hasCycle(graphCopy);
    }

    void printDeadlockStatus(const std::unordered_map<int, std::unordered_set<int>>& graph) {
        bool deadlockDetected = detectDeadlock(graph);
        
        std::cout << "Deadlock Detection Results:" << std::endl;
        std::cout << "Deadlock Exists: " << (deadlockDetected ? "Yes" : "No") << std::endl;

        // Print resource allocation details
        std::cout << "\nResource Allocation Details:" << std::endl;
        for (const auto& process : processes) {
            std::cout << "Process " << process.getId() << ":" << std::endl;
            std::cout << "  Allocated Resources:" << std::endl;
            for (const auto& [resourceId, instances] : process.getAllocatedResources()) {
                std::cout << "    Resource " << resourceId << ": " << instances << " instances" << std::endl;
            }
        }
    }
};

int main() {
    // Input resources in main function
    std::vector<Resource> resources = {
        Resource(1, 3),   // Resource 1 has 3 instances
        Resource(2, 2),   // Resource 2 has 2 instances
        Resource(3, 1)    // Resource 3 has 1 instance
    };

    // Input processes in main function
    std::vector<Process> processes = {
        Process(0),
        Process(1),
        Process(2)
    };

    // Simulate resource requests and allocations in main function
    processes[0].requestResource(1, 2);   // Process 0 requests 2 instances of Resource 1
    processes[1].requestResource(2, 1);   // Process 1 requests 1 instance of Resource 2
    processes[2].requestResource(3, 1);   // Process 2 requests 1 instance of Resource 3

    processes[0].allocateResource(1, 2);  // Allocate requested resources
    processes[1].allocateResource(2, 1);
    processes[2].allocateResource(3, 1);

    // Create deadlock graph in main function
    std::unordered_map<int, std::unordered_set<int>> graph;
    graph[0].insert(1);  // Process 0 waiting for Process 1
    graph[1].insert(2);  // Process 1 waiting for Process 2
    graph[2].insert(0);  // Process 2 waiting for Process 0

    // Create DeadlockDetector with input from main
    DeadlockDetector detector(resources, processes);
    
    // Detect and print deadlock
    detector.printDeadlockStatus(graph);

    return 0;
}
