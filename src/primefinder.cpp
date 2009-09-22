//////////////////////////////
// primefinder.cpp
//
#include "primefinder.h"
#include <cmath>

using namespace bplus;
using namespace bplus::service;
using namespace std;


PrimeFinder::PrimeFinder( const Transaction& tran, const Map& args ) :
    m_quantityToFind( args["quantity"] ),
    m_reportingInterval( args["interval"] ),
    m_cb( tran, dynamic_cast<const CallBack&>(args["callback"]) ),
    m_thrd()
{
}


PrimeFinder::~PrimeFinder()
{
    // TODO: review.
    m_thrd.join();

    // TODO: call tran.complete?
}


void PrimeFinder::run()
{
    m_thrd.run( threadFunc, this );
}


void* PrimeFinder::threadFunc( void* cookie )
{
    PrimeFinder* fndr = (PrimeFinder*) cookie;
    fndr->findPrimes_naive();
    return NULL;
}



// Calculate primes using trial division.
// This is a naive implementation with very little optimization.
// See: http://en.wikipedia.org/wiki/Primality_test
//
void PrimeFinder::findPrimes_naive()
{
    long long quantityFound = 0;
    for (long long n = 2; quantityFound < m_quantityToFind; ++n) {
        bool bIsPrime = true;
        long long maxDivisor = (long long) floor( sqrt( (long double) n ) );

        for (long long divisor = 2; divisor <= maxDivisor; divisor++) {
            if (n % divisor == 0) {
                bIsPrime = false;
                break;
            }
        }

        if (bIsPrime) {
            quantityFound++;
            if (quantityFound % m_reportingInterval == 0) {
                reportPrime( quantityFound, n );
            }
        }
    }
}


void PrimeFinder::reportPrime(  long long position, long long value )
{
    // Send results to our onHop() on the original transaction thread.
    // onHop will free this data.
    Map* pmArgs = new Map;
    pmArgs->add( "position", Integer(position).clone() );
    pmArgs->add( "value", Integer(value).clone() );

    hop( pmArgs );
}


void PrimeFinder::onHop( void* context )
{
    Map* pmArgs = (Map*) context;

    m_cb.invoke( *pmArgs );
    
    delete pmArgs;
}


