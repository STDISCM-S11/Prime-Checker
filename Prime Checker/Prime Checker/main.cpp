#include <chrono>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

#define LIMIT 10000000

/*
This function checks if an integer n is prime.

Parameters:
n : int - integer to check

Returns true if n is prime, and false otherwise.
*/

bool check_prime(const int& n);
void primesCheck(int start, int end, std::vector<int>& primes,
    std::mutex& mutex);

int main() {
    // initialize
    int threadCount = 1;
    int limit = LIMIT;

    // check for input
    std::cout << "Enter the upper limit: ";
    std::cin >> limit;
    std::cout << "Enter the number of threads: ";
    std::cin >> threadCount;

    const int numThreads = threadCount;

    std::vector<int> primes;
    std::vector<std::thread> threads;
    std::mutex mutex;

    auto start_time = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < threadCount; i++) {
        int start = i * (LIMIT / threadCount) + 2;
        int end = (LIMIT / threadCount) * (i + 1) + 2;

        if (i == threadCount - 1) {
            end = LIMIT + 1;
        }
        threads.push_back(std::thread(primesCheck, start, end, 
            std::ref(primes), std::ref(mutex)));
    }

    for (int i = 0; i < threadCount; i++) {
        threads[i].join();
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end_time - start_time;

    std::cout << primes.size() << " primes were found." << std::endl;
    std::cout << "Execution time: " << duration.count() << " seconds." 
        << std::endl;

    return 0;
}

void primesCheck(int start, int end, std::vector<int>& primes,
    std::mutex& mutex) {
    for (int current_num = start; current_num <= end; current_num++) {
        if (check_prime(current_num)) {
            std::unique_lock<std::mutex> lock(mutex);
            primes.push_back(current_num);
            lock.unlock();
        }
    }
}

bool check_prime(const int& num) {
    for (int i = 2; i * i <= num; i++) {
        if (num % i == 0) {
            return false;
        }
    }
    return true;
}