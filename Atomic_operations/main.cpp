#include <iostream>
#include <thread>
#include <atomic>
#include <vector>
#include <chrono>

/*
 * - Member Functions for Atomic Types
 *
 * - store()
 *      - Atomically replace the object's value with its argument
 * - load()
 *      - Atomically return the object's value
 * - operator=()
 * - operator T()
 *      - Synonyms for store() and load()
 * - exchange()
 *      - Atomically replace the object's value with its argument
 *      - Returns the previous value
 *      */

/*
 * - Member Functions for Specializations
 *
 * - Atomic pointers support pointer arithmetic
 *      - increment and decrement operators
 *      - fetch_add() synonym for x++
 *      - fetch_sub() synonym for x--
 *      - += and -= operators
 *
 * - Integer specializations have these, plus
 *      - Atomic bitwise logical operations &, | and ^
 *      */

/*
 * std::atomic_flag
 * - std::atomic_flag is an atomic boolean type
 *      - Has less overhead than std::atomic<bool>
 *
 * - Only three operations
 *      - clear() sets flag to false
 *      - test_and_set() sets flag to true and returns previous value
 *      - operator =()
 *
 * - Must be initialized to false
 *      atomic_flag lock = ATOMIC_FLAG_INIT
 *      */

/*
 * Spin Lock
 * - A spin lock is essentially an infinite loop
 *      - it keeps "spinning" until a condition becomes true
 * - An alternative to locking a mutex or using a condition variable
 * - We can use std::atomic_flag to implement a basic spin lock
 *      - The loop condition is the value of the flag
 *      */

/*
 * Spin Lock with std::atomic_flag
 *
 * - Each thread calls test_and_set() in a loop
 * - if this returns true
 *      - Some other thread has set the flag and is in the critical section
 *      - Iterate again
 * - if it returns false
 *      - This thread has set the flag
 *      - Exit the loop and proceed into the critical section
 * - After the critical section, set the flag to false
 *      - This allows another thread to execute in the critical section
 * */

/*
 * - Spin lock sample code
 *
 *          // Initialize flag to false
 *          std::atomic_flag flag = ATOMIC_FLAG_INIT;
 *
 *          void task(int n)
 *          {
 *              // Loop until we can set the flag
 *              while(flag.test_and_set()) {}
 *
 *              // Critical section
 *              ....
 *
 *              // Clear the flag
 *              flag.clear();
 *          }
 *          */

/*
 * Pros and cons of spin lock
 *
 * - A spinning thread remains active
 *      - A mutex may block the thread
 * - It can continue immediately when it "gets the lock"
 *      - With a mutex, the thread may need to be reloaded or woken up
 *
 * - Processor-intensive
 *      - Only suitable for protecting very short critical sections
 *      - And/or very low contention
 *      - Performance can be heavily impacted if spinning threads interrupt each other
 *      - Usually only used in Operating systems and libraries
 *      */

/*
 * Hybrid Mutex
 * - Often used to implement std::mutex
 * - Start with a spin lock with a timeout
 *      - If the threads sets the flag in time, enter the critical section
 *      - If the thread cannot set the flag in time, use the normal mutex implementation
 *
 * - This gives better performance than the conventional implementation
 * */

/*
 * Lock-free Programming
 *
 * - Threads execute critical sections concurrently
 *      - Without data races
 *      - But without using the operating system's locking facilities
 *
 * - Avoids or reduces some of the drawbacks to using locks
 *      - Race conditions caused by forgetting to lock, or using the wrong mutex
 *      - Lack of composability
 *      - Risk of deadlock
 *      - High overhead
 *      - Lack od scalability caused by coarse-grained locking
 *      - code complexity and increased overhead caused by fine-grained locking
 *      */

/*
 * Locking vs Lock-free
 *
 * - Both programming styles are used to manage shared state
 *      - Analogous to managing a traffic intersection
 *
 * - Locks
 *      - Traffic lights control access
 *      - Stop and wait until able to proceed into critical section
 *      */

/*
 * - Lock-free
 *      - Motorway-style intersection
 *      - Traffic from different levels can go over the same section at the same time
 *      - Traffic from one level can merge with traffic from a different level without stopping
 *      - If not done carefully, collisions can occur!
 *      */

/*
 * Advantages of Lock-free Programming
 * - If done correctly, threads can never block each other
 *      - No possibility of deadlock or livelock
 *      - If a thread is blocked, other threads can continue to execute
 *      - Useful if work must be completed within a time limit
 *      - (e.g. real time systems)
 *      */

/*
 * Drawbacks of Lock-free Programming
 *
 * - Very difficult to write code which is correct and efficient
 * - The extra complexity makes it unsuitable for many applications
 *      - e.g. User interface code with separation of concerns
 *      - May be useful in performance-critical code, such as infrastructure
 *
 * - Should be used only if
 *      - A data structure in the program is subject to high contention
 *      - Which causes unacceptable performance
 *      - And the lock-free version brings performance up to acceptable levels
 *      */

/*
 * The Everyday World of Programming with Locks
 *
 * - We can make some very useful assumptions
 * - Global state is consistent
 *      - Provided we only access shared data inside a locked region
 *      - No other threads will see our changes
 *      - Until the lock is released
 *
 * - Logical consistency
 *      - When working inside a locked region, global state will not change
 *      - e.g. between evaluating an "if" statement and executing the body
 *
 *
 * - Code order
 *      - Statements will execute in the same order as in the source code
 *      - Or at least, they will appear to...
 *      */

/*
 * The Strange World of Lock-free Programming
 * - None of these assumptions apply to lock-free programs
 *      - Shared data may have different values in different threads
 *      - The value may change between an "if" statement and its body
 *      - Statements may execute in a different order from the source code
 *      */

/*
 * Transactions
 *
 * - Transactional model of lock-free programming
 *      - "ACID"
 * - Atomic/All-or-nothing
 *      - A transaction either completes successfully("commit")
 *      - Or it fails and leaves everything as it was ("rollback")
 * - Consistent
 *      - The transaction takes the database from one consistent state to another
 *      - As seen by other users, the database is never in an inconsistent state
 *      */

/*
 * - Transactional model of lock-free programming
 *      - "ACID"
 * - Isolated
 *      - Two transactions can never work on the same data simultaneously
 * - Durable
 *      - Once a transaction is committted, it cannot be overwritten
 *      - ....until the next transaction sees the result of the commit
 *      - There is no possibility of "losing" an update
 *      */

/*
 * Transactional Memory
 * - Put shared data in transactional memory
 * - All operations on shared data will be transactional
 * - However, there is no standard implementation in C++
 * */

/*
 * Atomic Instructions
 * - Atomic/All-or-nothing
 *      - An instruction completes successfully(we hope!)
 * - Consistent
 *      - The instruction takes data from one consistent value to another
 *      - As seen by other threads, the data never has an inconsistent value
 *
 * - Isolated
 *      - Two atomic instructions can never work on the same data simultaneously
 *
 * - Durable
 *      - Once an instruction is completed, the data cannot be overwritten
 *      - ...until the next instruction sees the result
 *      - There is no possibility of "losing" a modification
 *      */

/*
 * Lock-free Programming
 * - To achieve this lock-free programming
 *      - Use atomic instructions
 * - We need to think carefully about thread interactions
 *      - Other threads can interleave between each statement
 *      - Or between expressions within statements
 *      - How do concurrent writers interact with each other?
 *      - How do concurrent writers interact with concurrent readers?
 *      */



// The atomic_flag must be initialized as false
std::atomic_flag lock_cout = ATOMIC_FLAG_INIT;

void task(int n)
{
    // test_and_set()
    // Returns true if another thread set the flag
    // Returns false if this thread set the flag

    while(lock_cout.test_and_set()) {}

    // Start of critical section
    // do some work
    using namespace std::literals;
    std::this_thread::sleep_for(50ms);
    std::cout << "I'm a task with argument " << n << std::endl;
    // End of critical section

    // Clear the flag, so another thread can set it
    lock_cout.clear();

}

// same code utilizing a mutex
std::mutex mut;
void task_m(int n)
{
    std::lock_guard<std::mutex> lg(mut);

    // Start of critical sections
    // do some work
    using namespace std::literals;
    std::this_thread::sleep_for(50ms);
    std::cout << "I'm a task with argument " << n << std::endl;
    // End of critical section
}
int main() {
//    std::cout << "Hello, World!" << std::endl;
//
//    std::atomic<int> x = 0;
//    std::cout << "After initialization: x =  " << x << std::endl;
//
//
//    // Atomic assignment to x
//    x = 2;
//
//    // Atomic assignment from x, y can be non-atomic
//    int y = x;
//
//    std::cout << "After assignment: x = " << x << ", y = " << y << std::endl;
//
//    x.store(3);
//    std::cout << "After store: x = " << x.load() << std::endl;



    std::vector<std::thread> threads;

    for (int i = 0; i <= 10; ++i) {
        threads.push_back(std::thread(task, i));
    }

    for (auto &thread : threads) {
        thread.join();
    }
    return 0;
}
