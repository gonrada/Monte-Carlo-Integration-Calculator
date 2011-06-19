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
double * tSum, * tSumSq;
uint64_t * tNumPnts; // number on random points each thread is to try

/* 
 *  Function Prototypes
 */
    // this will be the integrand function
inline double integrand(double x) { return exp(-x*x); }
void * compute_sum(void *);


int main(int argc, char **argv)
{
    bool okToStop = false;
    double err_achieved=0.1, err_desired=0, nSum=0, nSumSq=0, fHat,fSqHat;
    mode opMode;
    pthread_t thread_ids[NUMTHREADS];
    time_t start, end;
    uint64_t i=0, n=NUMTHREADS;
    

    tSum = new double[NUMTHREADS];
    tSumSq = new double[NUMTHREADS];
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
            err_desired = atof(argv[2]);
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
            //n = n * (100 * (1 - (err_desired / err_achieved)));
            n = (err_achieved / err_desired) >= 0.1 ? (n * 100) : (n * ((err_achieved / err_desired) / 0.1));
        
            for(int t=0; t < NUMTHREADS; ++t)
                tNumPnts[t] = n/NUMTHREADS;
            tNumPnts[0] += (n - (NUMTHREADS * tNumPnts[0]));
        }
        else
        {
            for(int t=0; t < NUMTHREADS; ++t)
                tNumPnts[t] = n/NUMTHREADS;
            tNumPnts[0] += (n - (NUMTHREADS * tNumPnts[0]));
        }

        for(int t=0; t < NUMTHREADS; ++t)
        {
            int * tid = new int(t);
            if(tNumPnts[t] > 0)
                pthread_create(&thread_ids[t], NULL, compute_sum, (void*) tid);
            delete(tid);
        }

        for(int t=0; t < NUMTHREADS; ++t)
            if(tNumPnts[t] > 0)
                pthread_join(thread_ids[t], NULL);

        i += n;
        for(int t=0; t < NUMTHREADS; ++t)
        {
            nSum += tSum[t];
            nSumSq += tSumSq[t];
            tSum[t] = tSumSq[t] = 0;
        }
        fHat = nSum / i;
        fSqHat = nSumSq / i;

        err_achieved = sqrt((fSqHat-(fHat*fHat))/i);

        if(opMode == error)
        {
            if(err_achieved <= err_desired) // if the achieved err is < 
                okToStop = true;       
        }
        else // opMode == points
        {
            okToStop = true; // we have finished all points
        }
    } while(!okToStop);
    end = time(0);
    cerr<<"Value:"<<fHat<<endl;
    cerr<<i<<" random points; error:"<<err_achieved<<endl;
    cerr<<"time elapsed:"<<end-start<<" sec.\n";

    return ;
}

void * compute_sum ( void * tPtr)
{
    int tid = *(int *) tPtr;
    double fVal=0;
    
    mt19937 ranSeed(static_cast<unsigned int>(time(0)* (tid+1)));
    uniform_01<mt19937&> distNumGen(ranSeed);

    for(uint64_t i=0; i < tNumPnts[tid]; ++i)
    {
        fVal = integrand(distNumGen()); // makes a single call to the integral funtion
        tSum[tid] += fVal;
        tSumSq[tid] += fVal * fVal;
    }
    return NULL;
}
