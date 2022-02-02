/**
 * \file        dot_product_parallel.cc
 * \brief       Simple program used to benchmark dot product operation with different parallel
 *              threading strategies.
 * \author      P. Prunty
 * \version     1.0
 * \date        2020-05-02
**/
#include <vector>
#include <iostream>
#include <chrono>
#include <functional>
#include <random>
#include <algorithm>
#include <thread>
#include <future>
#include <unistd.h>
#include <sys/wait.h>

// The number of threads for parallel computation. This program is set up to use 3 and only 3 threads.
unsigned available_threads = 3;

// Generic lambda which acts as a wrapper for std::inner_product
auto partial_dot = [](auto it, auto it2, auto it3) { return std::inner_product(it, it2, it3, 0.0); };

// You can use this global to make it easy to switch between async and deferred thread policies
//const auto policy = std::launch::async;
const auto policy = std::launch::deferred;

// Function prototypes
double serial_version(std::vector<double> v1, std::vector<double> v2);
double task_based(std::vector<double> v1, std::vector<double> v2);
double packaged_tasks(std::vector<double> v1, std::vector<double> v2);


/** \brief		            A simple function which uses a serial method to compute the dot product of two
 *                          vectors in parallel.
 *  \param v1               The first vector
 *  \param v2               The second vector
 *  \return dot_product     The dot product result
 */
double serial_version(std::vector<double> v1, std::vector<double> v2) {

    auto start = std::chrono::steady_clock::now();

    double dot_prod = std::inner_product(std::begin(v1), std::end(v1), std::begin(v2), 0.0);

    auto end = std::chrono::steady_clock::now();

    std::cout << "Dot product (serial). Answer = " << dot_prod << "\nElapsed time : " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms\n\n";

    return dot_prod;
}

/** \brief		            A simple function which uses a tasked based method to compute the dot product of two
 *                          vectors in parallel.
 *  \param v1               The first vector
 *  \param v2               The second vector
 *  \return dot_product     The dot product result
 */
double task_based(std::vector<double> v1, std::vector<double> v2) {

    auto chunk{static_cast<int>(v1.size() / available_threads)};  //<! Chunk size of vector for each process

    auto start = std::chrono::steady_clock::now();

    auto h0 = std::async(policy, partial_dot, v1.begin(), v1.begin() + chunk, v2.begin());
    auto h1 = std::async(policy, partial_dot, v1.begin() + chunk, v1.begin() + chunk * 2, v2.begin() + chunk);
    auto h2 = std::async(policy, partial_dot, v1.begin() + chunk * 2, v1.begin() + chunk * 3 + 1,
                         v2.begin() + chunk * 2);

    double dot_prod = h0.get() + h1.get() + h2.get();

    auto end = std::chrono::steady_clock::now();

    std::cout << "Dot product tasked based: " << dot_prod << "\nElapsed time : " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms\n\n";

    return dot_prod;
}

/** \brief		            A simple function which uses packaged tasks to compute the dot product of two
 *                          vectors in parallel.
 *  \param v1               The first vector
 *  \param v2               The second vector
 *  \return dot_product     The dot product result
 */
double packaged_tasks(std::vector<double> v1, std::vector<double> v2) {

    auto chunk{static_cast<int>(v1.size() / available_threads)};  //<! Chunk size of vector for each process

    // Define the task that will be done on each thread. i.e a partial dot prodct
    using task_type = double(std::vector<double>::iterator, std::vector<double>::iterator,
                             std::vector<double>::iterator);

    // Package the task. i.e what will each thread be tasked with doing?
    std::packaged_task <task_type> pt0{partial_dot};
    std::packaged_task <task_type> pt1{partial_dot};
    std::packaged_task <task_type> pt2{partial_dot};

    std::future<double> f0{pt0.get_future()};   //<! Get hold of t0's future
    std::future<double> f1{pt1.get_future()};   //<! Get hold of t1's future
    std::future<double> f2{pt2.get_future()};   //<! Get hold of t2's future


    auto start = std::chrono::steady_clock::now();

    std::thread t0{std::move(pt0), v1.begin(), v1.begin() + chunk, v2.begin()};
    std::thread t1{std::move(pt1), v1.begin() + chunk, v1.begin() + chunk * 2, v2.begin() + chunk};
    std::thread t2{std::move(pt2), v1.begin() + chunk * 2, v1.begin() + chunk * 3 + 1, v2.begin() + chunk * 2};

    double dot_prod = f0.get() + f1.get() + f2.get();

    auto end = std::chrono::steady_clock::now();

    std::cout << "Dot Product parallel threads & packaged task: " << dot_prod << "\nElapsed time : " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms\n\n";

    t0.detach();
    t1.detach();
    t2.detach();

    return dot_prod;
}


int main(void) {

    // Set size of vectors
    long n;

    std::cout << "Enter n the size of the two vectors v1 and v2 to perform dot product operation v1.v2: ";
    std::cin >> n;

    // Set up rngs
    std::random_device rd{};
    std::default_random_engine eng{4};
    auto ui = std::bind(std::normal_distribution<>{}, std::ref(eng));

    // Initialize and populate two vectors for dot product operation
    std::vector<double> v1(n);
    std::vector<double> v2(n);
    std::generate(std::begin(v1), std::end(v1), ui);
    std::generate(std::begin(v2), std::end(v2), ui);

    // Print number of threads on machine
    auto hardware_threads = std::thread::hardware_concurrency();
    std::cout << "Using  " << available_threads << " out of " << hardware_threads << " hardware threads" << "\n\n";

    serial_version(v1, v2);
    task_based(v1, v2);
    packaged_tasks(v1, v2);

}
