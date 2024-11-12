#include <iostream>
#include <unordered_map>
#include <list>
#include <queue>
#include <vector>
#include <unordered_set>

using namespace std;

class PageReplacement {
protected:
    int capacity;

public:
    PageReplacement(int capacity) : capacity(capacity) {}
    virtual bool accessPage(int page) = 0;  // Returns true if page fault occurs
    virtual void displayPages() const = 0;
};

// FIFO Page Replacement
class FIFO : public PageReplacement {
    queue<int> pages;
    unordered_set<int> pageSet;

public:
    FIFO(int capacity) : PageReplacement(capacity) {}

    bool accessPage(int page) override {
        if (pageSet.find(page) != pageSet.end()) {
            return false; // Page is already in memory, no page fault
        }
        if (pages.size() == capacity) {
            int oldest = pages.front();
            pages.pop();
            pageSet.erase(oldest);
        }
        pages.push(page);
        pageSet.insert(page);
        return true; // Page fault occurs
    }

    void displayPages() const override {
        queue<int> temp = pages;
        while (!temp.empty()) {
            cout << temp.front() << " ";
            temp.pop();
        }
        cout << endl;
    }
};

// LRU Page Replacement
class LRU : public PageReplacement {
    list<int> pages;
    unordered_map<int, list<int>::iterator> pageMap;

public:
    LRU(int capacity) : PageReplacement(capacity) {}

    bool accessPage(int page) override {
        if (pageMap.find(page) != pageMap.end()) {
            pages.erase(pageMap[page]);
            pages.push_front(page);
            pageMap[page] = pages.begin();
            return false; // No page fault
        }
        if (pages.size() == capacity) {
            int leastRecent = pages.back();
            pages.pop_back();
            pageMap.erase(leastRecent);
        }
        pages.push_front(page);
        pageMap[page] = pages.begin();
        return true; // Page fault occurs
    }

    void displayPages() const override {
        for (int page : pages) {
            cout << page << " ";
        }
        cout << endl;
    }
};

// LFU Page Replacement
class LFU : public PageReplacement {
    struct Page {
        int pageNum;
        int frequency;
        int timestamp; // To break ties in case of same frequency
    };

    struct CompareLFU {
        bool operator()(const Page& a, const Page& b) const {
            if (a.frequency == b.frequency)
                return a.timestamp > b.timestamp; // Use timestamp if frequency is same
            return a.frequency > b.frequency;
        }
    };

    int time;
    unordered_map<int, Page> pageMap;
    priority_queue<Page, vector<Page>, CompareLFU> minHeap;

public:
    LFU(int capacity) : PageReplacement(capacity), time(0) {}

    bool accessPage(int page) override {
        time++;
        if (pageMap.find(page) != pageMap.end()) {
            Page& entry = pageMap[page];
            entry.frequency++;
            entry.timestamp = time;
            minHeap.push(entry);
            return false; // No page fault
        }
        if (pageMap.size() == capacity) {
            while (!minHeap.empty() && pageMap[minHeap.top().pageNum].frequency != minHeap.top().frequency) {
                minHeap.pop(); // Remove stale entries
            }
            if (!minHeap.empty()) {
                pageMap.erase(minHeap.top().pageNum);
                minHeap.pop();
            }
        }
        Page newPage = {page, 1, time};
        pageMap[page] = newPage;
        minHeap.push(newPage);
        return true; // Page fault occurs
    }

    void displayPages() const override {
        for (const auto& [page, data] : pageMap) {
            cout << page << " ";
        }
        cout << endl;
    }
};

// Driver code to test the algorithms
int main() {
    int capacity = 3;
    vector<int> pageRequests = {1, 2, 3, 2, 1, 4, 5, 3, 2, 1, 4, 5};

    PageReplacement* algorithms[] = {new FIFO(capacity), new LRU(capacity), new LFU(capacity)};
    string names[] = {"FIFO", "LRU", "LFU"};

    for (int i = 0; i < 3; ++i) {
        cout << "Testing " << names[i] << " algorithm:\n";
        int pageFaults = 0;
        for (int page : pageRequests) {
            if (algorithms[i]->accessPage(page)) {
                pageFaults++;
            }
            algorithms[i]->displayPages();
        }
        cout << "Total Page Faults: " << pageFaults << "\n\n";
        delete algorithms[i];
    }
    return 0;
}
