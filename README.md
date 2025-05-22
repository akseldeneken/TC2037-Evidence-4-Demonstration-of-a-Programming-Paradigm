# TC2037-Evidence-4-Demonstration-of-a-Programming-Paradigm
## Description
In this evidence, I developed a script that simulates a concurrent file upload system, similar to the operation of platforms like Google Drive or Dropbox, where users can upload multiple files at the same time. This problem is relevant because in real systems, processing tasks one by one wastes time and resources. Allowing simultaneous uploads improves system efficiency and the user experience.

The solution was implemented using concurrent programming through threads (std::thread), which allows multiple tasks to be executed interleaved or simultaneously within the same program. Each thread represents a file being uploaded, and they all share resources such as available memory and the console.

To simulate a realistic limitation on system capacity, a semaphore was also implemented using mutexes and condition_variables to restrict the number of files that can be uploaded at the same time. This reflects scenarios in which the server has a limited number of active connections or available cores.

Concurrency was essential in this project because it allows for modeling real-life situations where multiple operations occur simultaneously and compete for the same resources. It also makes it easier to scale system behavior without completely blocking the execution of other processes.

## Model
The program is structured around a class called File, which represents a file to be uploaded. This class includes the main attributes of each file, such as its name, total size, the number of bytes uploaded so far, and its upload speed. It also includes the function that executes the upload process, which is called automatically when the corresponding thread is launched.

Each file is uploaded within a thread (std::thread). These threads run concurrently, meaning that all processes coexist and share system resources, such as total available memory and console output. The main function creates several File objects and launches a thread for each one.

To control access to shared memory and avoid race conditions, a mutex is used, ensuring that only one thread can modify certain data at a time (for example, when updating the upload progress or printing to the screen).

Additionally, to simulate a real server with limited capacity, a semaphore was implemented using a slot counter, an additional mutex, and a condition_variable. This allows only two files to be uploaded at a time. If a thread detects that there are no available slots, it waits until one becomes free. Upon completion of the upload, the thread frees its slot and wakes up another waiting thread.

In this way, the program reflects a realistic concurrent system, where several processes execute simultaneously, but in a controlled and synchronized manner.

![Diagrama en blanco (2)](https://github.com/user-attachments/assets/ba08e3cd-0391-4887-9ef4-66d314950681)

The diagram included in the evidence graphically represents this structure. It shows how multiple threads, each associated with a file, share the same memory space and I/O. Before executing the upload, all threads must go through a wait condition (cv.wait) that acts as an access filter, determined by the number of available slots.

Once a thread gains access, it simulates the upload process and, upon completion, releases the slot and notifies the next thread. This flow is reflected in the visual cycle diagram, where threads enter the concurrent process in a controlled manner, demonstrating how synchronization and resource management are implemented in a realistic concurrency environment.

## Implementation
The project was developed in C++ using the following standard libraries:

- #include <iostream> for input and output handling

- #include <thread> for creating multiple threads

- #include <mutex> for protecting shared resources

- #include <condition_variable> for implementing semaphore synchronization

- #include <chrono> for simulating wait times during file uploads

The main structure revolves around a class called File, which represents a file to be uploaded. Each File object contains its attributes (name, size, upload speed, etc.) and an operator() function that executes when the object is called from a thread. The function performs semaphore wait logic, memory verification, upload simulation with a progress bar, and resource release upon completion.

A key snippet of semaphore synchronization is as follows:
```
unique_lock<mutex> lock(mtxSlots);
cv.wait(lock, [](){ return slots > 0; });
slots--;
lock.unlock();

```
And at the end of the thread:
```
lock_guard<mutex> lock(mtxSlots);
slots++;
cv.notify_one();

```
This mechanism represents a semaphore-based concurrency control, ensuring that only a limited number of threads can upload files at the same time. The use of additional mutexes protects resources such as available memory and prevents race conditions.

## Tests
Several tests were performed to verify the program's concurrent behavior and validate the correct use of semaphores, memory, and synchronization between threads.

### Test 1: Uploading 5 files with different sizes and speeds
```
 thread t1(File("Word Document", 100, 0, 10));
    thread t2(File("Big Photo", 300, 0, 30));
    thread t3(File("Game", 500, 0, 50));
    thread t4(File("Video", 600, 0, 200));
    thread t5(File("Homework", 100, 0, 20));
```
Expected results:
- Only two files are uploaded at a time (via slots).
- The progress bar advances correctly in each thread.
- If there is not enough memory, the program displays the message:
"Insufficient storage space to upload file: Video."
- At the end, the remaining memory and details of each uploaded file are displayed.

Observations:
- Concurrency is clearly reflected in the interleaved output of the uploaded files.
- There were no console conflicts thanks to the use of mutexes.
- Memory was managed correctly, and files were uploaded until it ran out.

### Test 2: Changing the concurrency limit to 3

**Change:** `slots = 2` has been changed to `slots = 3`.

**Expectation:**
- Up to 3 files can be uploaded at the same time.
- Increased concurrency is observed in the output.
- The remaining threads wait their turn when there are already 3 active threads.
- Confirms that the semaphore adapts correctly to different limits.

**Expected result:**
- Three progress bars interleaved at the same time.
- Uploads complete faster compared to `slots = 2`.
- Memory is still respected and resources are freed upon completion.

## Test 3: Simulating insufficient memory from the start
Change: ```availableMemory = 100```
File: ```File("HugeFile", 300, 0, 50);```

Expectation:

- The file upload does not occur.
- Message appears: "Insufficient storage space to upload file: HugeFile"
 -The slot is correctly freed for other threads, if present.
