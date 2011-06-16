/*
    PROGRAM: Monte Carlo Integration Calculator
    AUTHOR: Sean Easton (gonrada@gmail.com)
    COURSE INFORMATION: CIS5930 - Summer '11
    Due Date: 6-20-2011

    SUMMARY
        This program will calculate integral using Monte Carlo methods.
        It is the goal of the project to parallelize said methods with the hope
        boosting overall performance.
*/

#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <ctime>
#include <boost/random/uniform_01.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <math.h>

using namespace std;
using boost::uniform_01;
using boost::mt19937;


/*
 *  Definitions
 */
#define NUMTHREADS 4
enum mode { points, error};

/*
 *  Global Variables
 */
double * tSum, tSumSq;
uint64_t * tNumPnts; // number on random points each thread is to try

/* 
 *  Function Prototypes
 */
    // this will be the integrand function
inline double integrand(double x) { return exp(-x*x); };
void * compute_sum(void *);


int main(int argc, char **argv)
{
    bool okToStop = false;
    double err, tSum, tSumSq;
    mode opMode;
    pthread_t thread_ids[NUMTHREADS];
    time_t start, end;
    uint64_t n = 1,i;
    

    tSum = new double[NUMTHREADS];
    tSumSp = new double[NUMTHREADS];
    tNumPnts = new uint64_t[NUMTHREADS];

    if (argc == 3)
    {
        if (strncmp(argv[1],"-n",2) == 0)
        {
            n = static_cast<unsigned int>(atoi(argv[2]));
            opMode = points;
        }
        else if (strncmp(argv[1],"-e",2) == 0)
        {
            err = atof(argv[2]);
            opMode = error;
        }
        else
        {
            n = 100000;
            opMode = points;
        }
    }
    else
    {
        n = 100000;
        opMode = points;
    }


    start = time(0);
    do
    {
        if(opMode == error)
        {
            n = n*(10*NUMTHREADS);
            for(int t=0; t < NUMTHREADS; ++t)
                tNumPnts[t] = n/NUMTHREADS; 
        }
        else
        {
            
        }

        for(int t=0; t < NUMTHREADS; ++t)
            pthread_create(&thread_ids[t], NULL, compute_sum, (void*) &t);

        for(int t=0; t < NUMTHREADS; ++t)
            pthread_join(thread_ids[t], NULL);

        if(opMode == points)
        {

        }
        else
        {

        }
    } while(!okToStop);
    end = time(0);

    cout<<i<<" "<<e<<endl;
    cerr<<i<<" random points; error:"<<e<<endl;
    cerr<<"time elapsed:"<<end-start<<" sec.\n";

    return 0;
}

void * compute_sum ( void * tPtr)
{
    int tid = *(int *) tPtr;

    mt19937 ranSeed(static_cast<unsigned int>(time(0)* (tid+1)));
    uniform_01<mt19937&> distNumGen(ranSeed);

    for(i=0; i < n; ++i)
    {
        fVal = integrand(distNumGen()); // makes a single call to the integral funtion
        tSum[tid] += fVal;
        tSumSq[tid] += fVal * fVal;
    }
}
