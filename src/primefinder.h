//////////////////////////////
// primefinder.h
//
#include "bpservice/bpcallback.h"
#include "bpservice/bptransaction.h"
#include "bputil/bpthread.h"
#include "bputil/bptypeutil.h"


class PrimeFinder
{
public:
    PrimeFinder( const bplus::service::Transaction& tran,
                 const bplus::Map& args );
    ~PrimeFinder();

    void run();
    
private:
    // Find requested number of primes.  Report them at requested interval.
    void findPrimes_naive();

    // Notify our client of a found prime.
    void reportPrime( long long position, long long value );

    // Notify our client that we've finished a transaction.
    void reportComplete();
    
    
private:
    long long                   m_quantityToFind;
    long long                   m_reportingInterval;
    bplus::service::Transaction m_tran;
    bplus::service::Callback    m_cb;
};


void launchFindOperation( const bplus::service::Transaction& tran,
                          const bplus::Map& args );

