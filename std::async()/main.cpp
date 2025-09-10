#include <iostream>
#include <future>
#include <iostream>
#include <chrono>

/*
 * std::async()
 *
 * - Defined in <future>
 * - Higher-level abstraction than std::thread
 *      - We can execute a task with std::async() which runs in the background
 *      - This allows us to do other work while the task is running
 *      - Alternatively, it can run synchronously in the same thread
 *
 * - Similar syntax to std::thread's constructor
 *      - The task function is the first argument
 *      - Followed by the arguments to the task function
 *      */

/*
 *  Hello, Async!
 *
 *          // The task function
 *          void hello()
 *          {
 *              std::cout << "Hello, Async! << std::endl;
 *          }
 *
 *          int main()
 *          {
 *              // Call std::async() to perform the task
 *              std::async(hello);
 *          }
 *          */
/*
 * std::async() with std::future
 *
 * - std::async() returns an std::future object
 *      - This contains the result of the task
 * - We can call get() on the future
 *      - Or wait() and friends
 * - This can be in a different thread from the call to std::async()
 * */

/*
 * Returning a Value
 *
 *          // Task which returns a value
 *          int func()
 *          {
 *              return 42;
 *          }
 *
 *          // Call async() and store the returned future object
 *          auto result = std::async(func);
 *
 *          // Do some other work
 *          ...
 *
 *          // Call get() when we are ready
 *          int answer = result.get();
 *          */

/*
 * std::async() and Exceptions
 * - The task may throw an exception
 * - The exception is stored in the future object
 * - It will be re-thrown when get() is called
 *      - Similar to using an explicit std::promise
 *      */

/*
 *
 *      std::async() and Exceptions
 *
 *      int produce()
 *      {
 *          int x = 42;
 *          // Code which may throw an exception
 *          ....
 *          return x;
 *      }
 *
 *      auto result = std::async(produce);
 *
 *      try {
 *          // Get the result - may throw an exception
 *          int x = result.get();
 *      }
 *      catch(std::exception& e) {
 *          // Handle the exception
 *          ...
 *      }
 *      */

// Task function
void hello()
{
    std::cout << "Hello, Async!" << std::endl;
}


// How to get a result from a task executed by std::async()
// Yet another bad Fibonacci implementation
int fib(int n) {
    if (n<= 1)
        return 1;
    return fib(n-1) + fib(n-2);
}

// Task which returns a value
unsigned long long fibonacci(unsigned long long n)
{
    if (n<= 1)
        return 1;
    return fibonacci(n-1) + fibonacci(n-2);
}

int produce()
{
    int x = 42;

    using namespace std::literals;
    std::this_thread::sleep_for(2s);

    // Some code which may throw an exception
    if (1) {
        throw std::out_of_range("Oops");
    }

    std::cout << "Produce returning " << x << std::endl;
    return x;
}
int main() {
//    std::cout << "Hello, World!" << std::endl;
//
//    // Call std::async() to perform the task
//    std::async(hello);
//    std::cout << fib(6) << std::endl;


    std::cout << "Calling fibonacci(44)" << std::endl;

    // Call async() and store the returned future
    auto result = std::async(fibonacci, 44);

    // Do some other work
    bool finished = false;
    using namespace std::literals;
    while (result.wait_for(1s) != std::future_status::ready) {
        std::cout << "Waiting for the result....." << std::endl;
    }

    // Call get() when we are ready
    std::cout << result.get() << std::endl;


    // Call async() and store the returned future
    auto result1 = std::async(produce);

    // Get the result - May throw an exception
    std::cout << "Future calling get()..." << std::endl;
    try{
        int x = result.get();
        std::cout << "Future returns from calling get()" << std::endl;
        std::cout << "The answer is " << x << std::endl;
    }

    catch(std::exception &e) {
        std::cout << "Exception caught: " << e.what() << std::endl;

    }
    return 0;
}
