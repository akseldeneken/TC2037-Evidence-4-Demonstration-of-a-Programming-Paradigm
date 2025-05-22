// Aksel Deneken Maldonado A01711966
// Evidence 4 Demonstration of a Programming Paradigm

#include <iostream>
#include <chrono>
#include <mutex>
#include <thread>
#include <condition_variable>


using namespace std;

// Total Memory Avaialble
// Test 3
// int availableMemory = 100;
int availableMemory = 1000;

condition_variable cv;

// Test 2
// int slots = 3;
int slots = 2;

// Mutex to protect shared variables
mutex mtx;
mutex mtxSlots;

// Class that represents a file to be uploaded
class File {
    public:
        string name;
        int totalSize;
        int uploaded;
        int speed;

        // Contructor
        File(string _name, int _totalSize, int _uploaded, int _speed) {
            name = _name;
            totalSize = _totalSize;
            uploaded = _uploaded;
            speed = _speed;
        }

        // Operator allows the File object to be used as a thread function.
        void operator()() {

            // Wait until theres an available slot
            unique_lock<mutex> lock(mtxSlots);
            cv.wait(lock, [](){ return slots > 0; });
            slots--;
            lock.unlock();

            // Measuring time
            auto start = chrono::steady_clock::now();

            // Variables to show progress
            int wideBar = 20;
            int percentage = 0;

            // Check if there's enough memory
            mtx.lock();
            if (availableMemory >= totalSize) {
                availableMemory -= totalSize;
                mtx.unlock();

                // Simulate file upload
                while (uploaded <= totalSize){
                    mtx.lock();

                    int progressBar = (percentage * wideBar) / 100;
                    int notProgress = wideBar - progressBar;
                    string bar = string(progressBar, '#') + string(notProgress, '-');

                    cout << "Uploading File: " << name << " | File Size: " << totalSize << " | Progress: " << bar << " " << percentage << "%" << endl;
                    this_thread::sleep_for(chrono::seconds(1));
                    mtx.unlock();

                    // Update Progress
                    if (uploaded + speed > totalSize){
                        uploaded = totalSize;
                        break;
                    } else {
                        uploaded += speed;
                        percentage = (uploaded * 100) / totalSize;
                    }

                }

                mtx.lock();

                // Measuring time
                auto end = chrono::steady_clock::now();
                auto duration = chrono::duration_cast<chrono::milliseconds>(end - start).count();

                cout << "File: '" << name << "' uploaded succesfully!" << endl;
                cout << "Size: " << totalSize << "Bytes |" << " Time: " << duration/1000 << "s |" << " Speed: " << speed << "bytes/s" << endl;

                mtx.unlock();

                // Release slot and notify other threads
                lock_guard<mutex> lock(mtxSlots);
                slots++;
                cv.notify_one();

            } else {
                // Not enough memory
                cout << "Insufficient storage space to upload file: "  << name << endl;
                mtx.unlock();

                // Release slot and notify others
                lock_guard<mutex> lock(mtxSlots);
                slots++;
                cv.notify_one();
                return;
            }
        }
};


int main() {
    // Test 1:
    thread t1(File("Word Document", 100, 0, 10));
    thread t2(File("Big Photo", 300, 0, 30));
    thread t3(File("Game", 500, 0, 50));
    thread t4(File("Video", 600, 0, 200));
    thread t5(File("Homework", 100, 0, 20));

    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();

    cout << "Memory left: " << availableMemory << endl;

    return 0;
}