#include <thread>
#include <chrono>
#include <iostream>

#include <condition_variable>
#include <mutex>
#include <queue>

std::queue<int> events;
std::mutex mtx;
std::condition_variable condition;

bool running = true;

void presser() {
    while(running) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "Button pressed";

        { //apparently, need to create a new scope to make this mutex get removed as soon as we are done our operations
            std::lock_guard<std::mutex> lock(mtx); // handles mutex
            events.push(1);
        }
        condition.notify_one();
    }
}

void consumer() { //consumes things from the queue
    std::cout << "Button X pressed at " << std::chrono::steady_clock::now() << " ms"; //replace this with actual time


    while (running) {
        std::unique_lock<std::mutex> lock2(mtx);
        condition.wait(lock2, [] {return !events.empty() || !running;});
        if (!running && events.empty()) {
            break;
        }
        int front_elem = events.front();
        events.pop();

        lock2.unlock();
        std::cout << "Event consumed: " << front_elem;
    }
}

void heartbeat(int time) {
    int count = 0;
    while (count < time) {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        std::cout << "Heartbeat\n";
        //now, i need to someone store an event in a queue with a mutex thing??
        count++;
    }
}

int main() {
    std::thread button_presser(presser);
    std::thread consumer_thread(consumer);
    std::thread heartbeat_thread(heartbeat, 6);

    std::this_thread::sleep_for(std::chrono::seconds(3));

    running = false;
    condition.notify_all();

    button_presser.join();
    consumer_thread.join();
    heartbeat_thread.join();

    return 0;
}