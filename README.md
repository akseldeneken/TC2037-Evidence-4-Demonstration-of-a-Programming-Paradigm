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
