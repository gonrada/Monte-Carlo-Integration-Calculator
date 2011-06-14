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
    unsigned int n = 0; // number of random points
    double nSum = 0, nSumSq = 0, err = 0, e = 1, r;
    double fHat, fSqHat;;

    if (argc == 3)
    {
        if (strncmp(argv[1],"-e",2) == 0)
        {
            err = atof(argv[2]);
        }
        else
        {
            n = static_cast<unsigned int>(atoi(argv[2]));
        }
    }
    else
    {
        n = 100000;
    }

//    cout<<"got here; n:"<<n<<" err:"<<err<<endl;

    for(int i=2; i != n && e > err; i+=2)
    {
        r = dist(); // get a random value

        nSum += exp(-r*r);
        nSumSq += exp(-r*r) * exp(-r*r);

        r = dist(); // get a random value

        nSum += exp(-r*r);
        nSumSq += exp(-r*r) * exp(-r*r);


        //http://math.fullerton.edu/mathews/n2003/montecarlomod.html

        fHat = nSum / i;
        fSqHat = nSumSq / i;

        e = sqrt((fSqHat-(fHat*fHat))/i);

        cout<<i<<" random points; error: "<<e<<endl;
    }

    return 0;
}
