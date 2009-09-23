//////////////////////////////
// primefinder.h
//
#include "bpservice/bpcallback.h"
#include "bpservice/bptransaction.h"
#include "bputil/bpthread.h"
#include "bputil/bpthreadhopper.h"
#include "bputil/bptypeutil.h"


class PrimeFinder : bplus::thread::HoppingClass
{
public:
    PrimeFinder( const bplus::service::Transaction& tran,
                 const bplus::Map& cb );
    ~PrimeFinder();

    void run();
    
private:
    // Thread func, will receive this pointer as arg.
    static void* threadFunc( void* cookie );

    // Find requested number of primes.  Report them at requested interval.
    void findPrimes_naive();

    // Notify our client of a found prime.
    void reportPrime( long long position, long long value );

    // Notify our client that we've finished a transaction.
    void reportComplete();
    
    // Handle data sent to us from the thread we spawn.
    virtual void onHop( void* context );
    
private:
    long long                   m_quantityToFind;
    long long                   m_reportingInterval;
    bplus::service::Transaction m_tran;
    bplus::service::Callback    m_cb;
    bplus::thread::Thread       m_thrd;
};


