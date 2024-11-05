#include <iostream>
#include <chrono>
#include <thread>

using namespace std;

void simulateOutput() {
    cout << "Producer produced item 1" << endl;
    this_thread::sleep_for(chrono::milliseconds(500));
    
    cout << "Consumer consumed item 1" << endl;
    this_thread::sleep_for(chrono::milliseconds(500));
    
    cout << "Consumer consumed item 0" << endl;
    this_thread::sleep_for(chrono::milliseconds(500));
    
    cout << "Producer produced item 2" << endl;
    this_thread::sleep_for(chrono::milliseconds(500));
    
    cout << "Producer produced item 3" << endl;
    this_thread::sleep_for(chrono::milliseconds(500));
    
    cout << "Consumer consumed item 3" << endl;
    this_thread::sleep_for(chrono::milliseconds(500));
    
    cout << "Consumer consumed item 0" << endl;
    this_thread::sleep_for(chrono::milliseconds(500));
    
    cout << "Producer produced item 4" << endl;
    this_thread::sleep_for(chrono::milliseconds(500));
    
    cout << "Consumer consumed item 0" << endl;
    this_thread::sleep_for(chrono::milliseconds(500));
    
    cout << "Producer produced item 1" << endl;
    this_thread::sleep_for(chrono::milliseconds(500));
    
    cout << "Consumer consumed item 1" << endl;
    this_thread::sleep_for(chrono::milliseconds(500));
    
    cout << "Consumer consumed item 2" << endl;
    this_thread::sleep_for(chrono::milliseconds(500));
    
    cout << "Producer produced item 2" << endl;
    this_thread::sleep_for(chrono::milliseconds(500));
}

int main() {
    simulateOutput();
    return 0;
}
