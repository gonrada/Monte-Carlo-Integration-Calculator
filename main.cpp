/*
    PROGRAM: Monte Carlo Integration Calculator
    AUTHOR: Sean Easton (gonrada@gmail.com)
    COURSE INFORMATION: CIS5930 - Summer '11
    Due Date: 

    SUMMARY
        This program will calculate integral using Monte Carlo methods.
        It is the goal of the project to parallelize said methods with the hope
        boosting overall performance.
*/

#include "resources.h"

using namespace std;
using boost::uniform_01;

int main(int argc, char **argv)
{
    boost::mt19937 ranSeed(static_cast<unsigned int>(std::time(0)));
    uniform_01<boost::mt19937&> dist(ranSeed);

    cout<< dist()<<" "<<dist()<<"\n";

    return 0;
}
