#include <iostream>
#include <thread>
#include <chrono>
using namespace std;


class Semaphore
{
private:
    int S;

public:
    Semaphore(int value = 1)
    {
        S = value;
    }
    void wait()
    {
        while (S <= 0)
            S--;
    }
    void signal()
    {
        S++;
    }
};

const int BUFFER_SIZE = 4;          // Buffer size for items
int buffer[BUFFER_SIZE];            // Circular buffer
int in = 0;                         // Producer index
int out = 0;                        // Consumer index
Semaphore mutex(1);                 // Semaphore for mutual exclusion
Semaphore empty_slots(BUFFER_SIZE); // Semaphore to track empty slots
Semaphore full(0);                  // Semaphore to track full slots

void producer()
{
    while (true)
    {
        empty_slots.wait(); // Wait for an empty slot
        mutex.wait();       // Wait for mutual exclusion
        // Produce an item and place it in the buffer
        buffer[in] = in + 1; // Produce a value
        cout << "Producer produced item " << buffer[in] << endl;
        in = (in + 1) % BUFFER_SIZE;
        mutex.signal();                             // Signal that critical section is done
        full.signal();                              // Signal that a full slot is available
        this_thread::sleep_for(chrono::seconds(1)); // Simulate work been done
    }
}

void consumer()
{
    while (true)
    {
        full.wait();  // Wait for a full slot
        mutex.wait(); // Wait for mutual exclusion
        // Consume an item from the buffer
        cout << "Consumer consumed item " << buffer[out] << endl;
        buffer[out] = 0; // Reset the consumed slot (optional)
        out = (out + 1) % BUFFER_SIZE;
        mutex.signal();                             // Signal that critical section is done
        empty_slots.signal();                       // Signal that an empty slot is available
        this_thread::sleep_for(chrono::seconds(1)); // Simulate work been done
        
    }
}

int main()
{
    thread prod(producer);
    thread cons(consumer);
    prod.join();
    cons.join();
    return 0;
}