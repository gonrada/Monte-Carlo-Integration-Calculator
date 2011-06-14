/*
    PROGRAM: Monte Carlo Integration Calculator
    AUTHOR: Sean Easton (gonrada@gmail.com)
    COURSE INFORMATION: CIS5930 - Summer '11
    Due Date: 
*/

#ifndef DEFINITIONS_H

/*
 *  struct with the required info 
 *  for a thread to run
 */
struct tInfo {
    unsigned int tid; // thread id
    unsigned long numPnts; // number of random points a thread is to compute
    double nSum, nSumSq; // values returned to main thread
};





#endif
