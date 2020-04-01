/**********************************************
 * Last Name:   Nkwonta
 * First Name:  Sodienye
 * Student ID:  30000197
 * Course:      CPSC 457
 * Tutorial:    T05
 * Assignment:  3
 *********************************************/
/// compile with:
/// $ g++ countPrimes.cpp -O2 -o countPrimes -lm
/// Includes code from Dr. Hudson's countPrimes file

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

struct threadComponents {
    int n;
    int64_t num;
    int64_t start;
    bool result;// check if numbers are prime
};

int64_t count;
int nThreads;
int64_t counter;

void * calcPrime(void *param) {
    struct threadComponents *n = (threadComponents *) param;
    int64_t n1 = n->num;// number in the file given to threadComponents used here

    // small numbers are not primes
    if (n1 <= 1) return 0; 

    // 2 and 3 are prime
    if (n1 <= 3) {
        n->result = 1;
        return 0;
    }

    // multiples of 2 and 3 are not primes
    if (n1 % 2 == 0 || n1 % 3 == 0) return 0; 
    //get the square root of the number
    int64_t max = (sqrt(n1));

    int64_t i = n->start;
    //printf("Index %lld primes.\n", n->startIndex);
    while(i <= max) {
        if (n1 % i == 0 || n1 % (i+2) == 0) {
            n->result = 0;
            return 0;
        }
        i += nThreads*6;
    }
    n->result = 1;
    return 0;
}

int isPrime(int64_t n)
{
  if( n <= 1) return 0; // small numbers are not primes
  if( n <= 3) return 1; // 2 and 3 are prime
  if( n % 2 == 0 || n % 3 == 0) return 0; // multiples of 2 and
  int64_t i = 5;
  int64_t max = sqrt(n);
    while(i <= max){
      if (n % i == 0 || n % (i+2) == 0) return 0;
      i += 6;
    return 1;
  }
  return 0;
}

//main function that creates the threads and calls the isPrime function
int main(int argc, char **argv) {
    /// parse command line arguments
    nThreads = 1;
    if (argc != 1 && argc != 2) {
        printf("Usage: countPrimes [nThreads]\n");
        exit(-1);
    }
    //set nThreads to be the number of threads passed
    if (argc == 2) nThreads = atoi(argv[1]);
    /// handle invalid arguments
    if (nThreads < 1 || nThreads > 256) {
        printf("Bad arguments. 1 <= nThreads <= 256!\n");
    }
    
    // setting up threads
    pthread_t thread[nThreads];
    struct threadComponents threadC[nThreads];

    count = 0;//number of primes
    
    while (1) {
        int64_t num;

        if (1 != scanf("%lld", &num)) {
            break;
        }
        for (int i = 0; i < nThreads; i++) {
            //found on stackexchange
            //threadC[i]=malloc(sizeof(threadComponents));
            threadC[i].result = 0;
            threadC[i].start = 5 + (i*6);
            threadC[i].num = num;
            threadC[i].n = i;
            pthread_create(&thread[i], NULL, calcPrime, (void *)&threadC[i]);//creating threads
        }
        

        //join all the threads
        // found on stackexchnage
        int next = 1;
        for(int i = 0; i< nThreads; i++) {
            pthread_join(thread[i], NULL);
            if (threadC[i].result == 0){
                next = 0;
            //free(threadC[i]);
            }
        }
        count += next;
    }

        /// count the primes
    printf("Counting primes using %d thread%s.\n",
            nThreads, nThreads == 1 ? "" : "s");

    /// report results
    printf("Found %lld primes.\n", count);
    exit(1);
    return 0;
}