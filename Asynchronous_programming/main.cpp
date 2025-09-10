#include <iostream>
#include <thread>
#include <chrono>
#include <future>



/*
 * Synchronous and Asynchronous Programming
 *
 * - Synchronous
 *      - Wait for each task to complete
 * - Asynchronous
 *      - Continue without waiting for tasks to complete
 *
 *
 *      */

/*
 * Synchronous Programming
 *
 * - A task starts another task
 * - The current task is blocked
 * - Must wait until the new task completes before it can continue
 *      - e.g. Synchronous database access
 *      - Do some work
 *      - Request data from the database
 *      - Wait for data
 *      - Receive data from database
 *      - Continue working
 *      */

/*
 * Synchronous Tasks
 *
 * - Normal function calls are synchronous tasks
 *          data.save(filename);
 *          // Stop and wait for func to return
 *          // ...wait...
 *          // Now we can continue with the next operation
 *
 * - We have to stop and wait for the save operation to complete
 *      - Even if the next operation does not depend on it
 * - This reduces throughput and user satisfaction
 *      - GUI applications appear unresponsive
 *      - Clients experience slow service
 *          */

/*
 * Asynchronous Programming
 * - A task starts another task
 * - The current task can continue
 * - The new task runs in the background
 *      - e.g. Asynchronous database access
 *      - Request data from database as a separate task
 *      - Do some more work in our task
 *      - Receive data from database
 *      */

/*
 * Asynchronous Tasks
 *
 * - Start off another task
 *      data.async_save(filename);
 *      // The asynchronous task runs in the background
 *      // We continue with the next operation
 *      // do something else
 *
 * - Our thread can continue its work
 *      // At some point, we may need to check if the async call has completed
 *      // Or to get its result
 *
 * */


/*
 * Advantages of Asynchronous Programming
 *
 * - The current task can do other work
 *      - Provided it does not require the data
 * - The current task only blocks when it needs the data
 *      - if the data is already available, it can continue without stopping
 * - This maintains throughput and user satisfaction
 *      - GUI applications appear responsive
 *      - Clients experience normal service
 *
 * */

/*
 * Blocking and Multi-threading Programs
 * - Blocking is undesirable in threaded programs
 *      - Blocking reduces throughput and responsiveness of the blocked thread
 *      - Any threads which join with this thread will also be blocked
 * - Particularly in a critical section
 *      - Any threads which are waiting to enter the critical section are also blocked
 *      - Possibility of deadlock, if we are using locks
 * - Using asynchronous programming reduces the need to block
 *      - But may not avoid it completely
 *      - e.g. if the database fetch is not complete when the data is needed
 *      */

/*
 * Blocking Synchronization
 *
 * - Blocking operations
 * - Synchronized by mutexes
 *      - A thread is blocked until another thread unlocks the mutex
 * - Or atomic operations
 *      - A thread is blocked until another thread completes an atomic operation
 *      */

/*
 * Non-blocking Synchronization
 *
 * - Non-blocking operations
 * - Synchronized by message queues
 *      - A thread pushes a message onto a concurrent queue
 *      - Another thread takes the message off the queue and processes it
 *      - Yhe first thread continues running without waiting
 *
 *  - The messages often consist of callable objects
 *      - The message is processed by invoking the callable object
 *
 * - C++ does not have a standard concurrent queue
 *      - Available in Boost, Microsoft's PPL, Intel's TBB
 *      */

/*
 * Asynchronous Programming and Parallelism
 *
 * - Asynchronous programming
 * - Can be used to perform parallel operations
 *      - Start new threads which all perform the same task
 *      - Collect the result from each thread as it completes its task
 *      - Combine the results into the final answer
 *
 * - It can also be used in single-threaded programs
 *      - Using operating system features
 *      */

/* Packaged Task
 * */

/*
 * std::packaged_task
 *
 * - Defined in <future>
 * - Encapsulated a task
 *      - A callable object for the task's code
 *      - An std::promise for the result of the task
 *
 * - Provides a higher level of abstraction
 * */

/*
 * - Template class
 *      - The parameter is the callable object's signature
 *          // std::packaged_task object
 *          // The callable object takes int and int arguments
 *          // it returns int
 *          std::packaged_task<int(int, int)>ptask(...);
 *
 * - The constructor takes the callable object as argument
 * */

/*
 * std::packaged_task Interface
 *
 * - Functor class
 * - Overloaded operator()
 *      - Invokes the callable object
 *      - Stores the return value in the promise object
 * - get_future()
 *      - Returns the std::future object associated with the promise
 *
 * - std::packaged_task is a move-only class
 * */

/*
 * Using an std::packaged_task Object
 *
 * - Pass a callable object to the constructor
 * - The packaged task starts when operator() is called
 *      - In the same thread, by calling it directly
 *      - In a new thread, by passing the task to std::thread's constructor
 * - We call get_future()
 * - We call get() on the returned future object
 *      - Or wait() and friends
 *      */


/*
 *
 * Examples of std::packaged_task in Same Thread
 *
 *          // Packaged task using lambda expressions
 *          std::packaged_task<int(int, int)> ptask([](int a, int b) {
 *              return a + b;
 *          });
 *
 *          // The future associated with the packaged_task's promise
 *          std::future<int> fut = ptask.get_future();
 *
 *          // Invoke the packaged task in this thread
 *          ptask(6, 7);
 *
 *          // Call get() to receive the result
 *          fut.get();
 *          */

/*
 * std::packaged_task
 * - it wraps any callable object(function, lambdas, function objects)
 * - it associates it with a std::future, allowing you to retrieve the result asynchronously
 * */

/*
 * Concept Overview
 * - View it as a task wrapper that packages a computation along with the future result of that computation
 * - When you invoke the task(calling it like a function), it stores the result internally
 * - You can retrieve this result through its associated std::future
 * */

/*
 * Example of std::packaged_task in New Thread
 *
 *      // Create a thread
 *      // The packaged task will be its entry point
 *      std::thread thr(std::move(ptask), 6, 7);
 *      thr.join();
 *      */

/* Advantages of std::packaged_task
 *
 * - Avoid boilerplate code
 *      - Create std::promise object
 *      - Pass it to task function
 *      - ETC
 *      */

/*
 * Applications of std::packaged_task
 * - Create a container of packaged_task objects
 *      - The threads do not start up until we are ready for them
 * - Useful for managing threads
 *      - Each task can be run on a specified thread
 *      - Thread scheduler runs threads in a certain order
 *      - Thread pool consists of threads waiting for work to arrive
 *      */
using namespace std::literals;

// Assignment
//std::promise<int> prom;
//std::future<int> fut = prom.get_future();

void produce(std::promise<int> &prom, int a, int b) {
    int result = a + b;
    prom.set_value(result);
}

void consume(std::future<int> &fut) {
    int result = fut.get();
    std::this_thread::sleep_for(2s);
    std::cout << "The final result is: " << result << std::endl;
}

int main() {
    std::cout << "Hello, World!" << std::endl;

    // Packaged task object
    // The callable object is a lambda expression
    // It takes int and int arguments
    // It returns int


    std::packaged_task<int(int, int)> ptask([](int a, int b) {
        std::this_thread::sleep_for(2s);
        return a + b;
    });

    // The promise object is an std::promise<int>
    // Get the future associated with it

    std::future<int> fut = ptask.get_future();

    // Invoke the packaged task in this thread
    ptask(6, 7);

    std::cout << "Waiting for result" << std::endl;

    // Call get() to receive the result of the packaged task
    std::cout << "6 + 7 is " << fut.get() << std::endl;

    // Assignment
    std::promise<int> promise;
    std::future<int> future = promise.get_future();


    std::thread producer(produce, std::ref(promise), 7, 8);
    std::thread consumer(consume, std::ref(future));

    // continue executing main
    std::cout << "Main does not stop running" << std::endl;
    for (int i = 0; i <= 30; ++ i) {
        if ((i % 3 == 0) && (i % 5 == 0)) {
            std::cout << "FizzBuzz" << std::endl;
        }
        else if(i % 3 == 0) {
            std::cout << "Fizz" << std::endl;
        }
        else if(i % 5 == 0) {
            std::cout << "Buzz" << std::endl;
        }
        else {
            std::cout << i << std::endl;
        }
    }

//    int result = future.get(); // blocks until produce() sets the value
//    std::cout << "Final result: " << result << std::endl;

    producer.join();
    consumer.join();


    return 0;
}
