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
    m_tran( tran ),
    m_cb( tran, args["callback"] )
{
}


PrimeFinder::~PrimeFinder()
{
}



// Calculate primes using trial division.
// This is a naive implementation with very little optimization.
// See: http://en.wikipedia.org/wiki/Primality_test
//
void PrimeFinder::run()
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

		// relinquish cpu a bit.
		Sleep(0);
    }

    reportComplete();
}


void PrimeFinder::reportPrime( long long position, long long value )
{
    Map args;
    args.add( "position", Integer(position).clone() );
    args.add( "value", Integer(value).clone() );

    m_cb.invoke( args );
}


void PrimeFinder::reportComplete()
{
    m_tran.complete( Bool(true) );
}



struct FindData
{
    FindData( const bplus::service::Transaction& tran_,
              const bplus::Map& args_ ) :
        tran( tran_ ),
        args( args_ )
    {}

    bplus::service::Transaction tran;
    bplus::Map args;            
};


void* calcAndReportPrimes( void* data )
{
    FindData* fd = (FindData*) data;
    
    PrimeFinder fndr( fd->tran, fd->args );
    fndr.run();

    return NULL;
}


void launchFindOperation( const Transaction& tran, const Map& args )
{
    FindData* fd = new FindData( tran, args );

    bplus::thread::Thread thrd;
    thrd.run( calcAndReportPrimes, fd );
    thrd.detach();
}



