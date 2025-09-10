#include <iostream>
#include <iostream>
#include <chrono>
#include <thread>
#include <string>
#include <future>
using namespace std::literals;

/*
 * Launch Options
 *
 * - std::async() may start a new thread for the task
 * - Or it may run the task in the same thread
 * - Controlled by the "Launch flag"
 *      - Optional argument to std::async()
 *      */

/*
 * Launch Flags
 * - std::launch::async
 *      - A new thread is started for the task
 *      - The task is executed as soon as the thread starts
 * - std::launch::deferred
 *      - Nothing happens until get() is called on the returned future
 *      - The task is then executed ("Lazy evaluation")
 * - if both flags are set
 *      - The implementation decides whether to start a new thread
 *      - This is the default
 *      */

/*
 * Default Launch Policy
 *
 * - Lack of certainty
 *      - The task could execute synchronously with the initiating thread
 *      - The task could execute concurrently with the initiating thread
 *      - It could execute concurrently with the thread that calls get()
 *      - if get() is not called, the task may not execute at all
 *
 * - Thread Local storage(TLS)
 *      - We do not know which thread's data will be used
 *      */

/*
 * Launch Policy Recommendations
 *
 * - Use the async launch option if any of these are true
 *      - The task must execute in a separate thread
 *      - The task must start immediately
 *      - The task will use thread-local storage
 *      - The task function must be executed, even if get() is not called
 *      - The thread receiving the future will call wait_for() or wait_until()
 *      */

/*
 * Use the deferred launch option if
 *      - The task must be run in the thread which calls get()
 *      - The task must be executed, even if no more threads can be created
 *      - You want lazy execution of the task
 *
 * - Otherwise, let the implementation choose
 * */

/*
 * Return Value from wait() and friends
 *
 * - wait()
 *      - Returns nothing
 * - wait_for()
 * - wait_until()
 *      - Return std::future_status::ready if the result is available
 *      - Return std::future_status::timeout if the timeout has expired
 *      - Return std::future_status::deferred if the result is being lazily evaluated
 *
 * - In lazy evaluation, the task does not run until get() is called
 * */

/*
 * Choosing a Thread Object
 *
 * - We now have three different ways to execute a task
 *      - Create an std::thread object
 *      - Create an std::packaged_task object
 *      - Call std::async()
 *      */

/*
 * - Advantages of std::async()
 *
 * - The simplest way to execute a task
 *      - Easy to obtain the return value from a task
 *      - Or to catch any exception thrown in the task
 *      - Choice of running the task synchronously or asynchronously
 *
 * - Higher-level abstraction than std::thread
 *      - The library manages the threads for the programmer
 *      - And the inter-thread communication
 *      - No need to use shared data
 *      */

/*
 * - Disadvantages of async()
 *
 * - Cannot detach tasks
 * - A task executed with std::launch::async is "implicitly joined"
 *
 *          {
 *              auto fut = std::async(std::launch::async, hello);
 *          } // Calls ~fut()
 * - The returned future's destructor will block
 *      -Until the task completes
 *      */

/*
 * - Advantages of std::packaged_task
 *
 * - The best choice if we want to represent tasks as objects
 *      - e.g. to create a container of tasks
 *
 * - A lower-level abstraction than std::async()
 *      - Can control when a task is executed
 *      - Can control on which thread it is executed
 *      */

/*
 * Advantages of std::thread
 * - The most flexible
 *      - Allows access to the underlying software thread
 *      - Useful for features not supported by standard C++
 *      - Can be detached
 *      */

/*
 * Recommendations
 *
 * - For starting a new thread in general
 *      - Use std::async()
 * - For containers of thread objects
 *      - Use std::packaged_tasks
 * - For starting a detachable thread
 * - For more specialized requirements
 *      - Use std::thread
 *      */

/*
 * - Books to read
 *      - Computer Systems: A Programmers Perspective
 *      - Operating Systems: Three Easy Pieces
 *      - C++ Concurrency In Action
 *      - The Linux Programming Interface
 *      - Developing The Linux Kernel
 *      */

int task()
{
    std::cout << "Executing task() in thread with ID: ";
    std::cout << std::this_thread::get_id() << std::endl;
    std::this_thread::sleep_for(5s);
    std::cout << "Returning from task()" << std::endl;
    return 42;
}

void func(const std::string &option = "default"s)
{
    std::future<int> result;

    if (option == "async"s) {
        result = std::async(std::launch::async, task);
    }
    else if (option == "deferred"s) {
        result = std::async(std::launch::deferred, task);
    }
    else {
        result = std::async(task);
    }

    std::cout << "Calling async with option \"" << option << "\"" << std::endl;
    std::this_thread::sleep_for(2s);
    std::cout << "Calling get()" << std::endl;
    std::cout << "Task result: " << result.get() << std::endl;
}
// async version
int produce(int a, int b) {
    std::cout << "Producing the value" << std::endl;
    return a * b;
}

void task1()
{
    std::this_thread::sleep_for(5s);
    std::cout << "Task result: " << 42 << std::endl;
}
void func1()
{
   std::cout << "Calling async" << std::endl;
   auto result = std::async(std::launch::async, task);

}


int multiply_by_2(int a) {
    return a * 2;
}


int main() {
    std::cout << "Hello, World!" << std::endl;


    // Using Async vs Packaged task

    std::packaged_task<int(int,int)> p_task([](int a, int b) {return a * b;});
    std::future<int> result = p_task.get_future();

    std::thread thr1 (std::move(p_task), 4, 6);
    // this thread continues to work
    for (int i = 0; i <= 10; ++i) {
        std::cout << i << std::endl;
    }
    std::cout << "packaged_task version result is: " << result.get() << std::endl;

    std::future<int> async_version = std::async(produce, 2, 7);
    // this thread continues to execute
    for (int i = 0; i <= 10; ++i) {
        std::cout << i << std::endl;
    }
    std::cout << "async version result value is: " << async_version.get() << std::endl;

    thr1.join();

    std::cout << "In main thread with ID: " << std::this_thread::get_id() << std::endl;

    func("async");
    func("deferred");
    func("default");

    int value = 200;
    int *ptr1 = &value;
    std::cout << ptr1 << std::endl;
    std::cout << &value << std::endl;
    std::cout << multiply_by_2(4) << std::endl;



    return 0;
}
