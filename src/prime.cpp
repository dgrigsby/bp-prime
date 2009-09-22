//////////////////////////////
// prime.cpp 
//
// Implements the Prime service.
//
#include "bpservice/bpservice.h"
#include "primefinder.h"

using namespace std;
using namespace bplus;
using namespace bplus::service;


class Prime : public Service
{
public:    
    BP_SERVICE( Prime );

    Prime() : Service() {}
    ~Prime() {}
    
    void    calc( const Transaction& tran, const Map& args );

private:
    
};


BP_SERVICE_DESC( Prime, "Prime", "0.0.1", "Calculates prime numbers." )
  ADD_BP_METHOD( Prime, calc, "Calculate prime numbers." )
    ADD_BP_METHOD_ARG( calc, "quantity", Integer, true,
                       "How many primes to calculate")
    ADD_BP_METHOD_ARG( calc, "interval", Integer, true,
                       "How often to report a found prime")
    ADD_BP_METHOD_ARG( calc, "callback", CallBack, true,
                       "JavaScript function that is notified of nth prime. "
                       "The data passed to the callback will be "
                       "{position:(Integer), value:(Integer)}" )
END_BP_SERVICE_DESC


          
////////////////////////////////////////////////////////////////////////////////
// Implementation
//

void Prime::calc( const Transaction& tran, const Map& args )
{
    // Create a prime finder that will do work in a new thread.
    // TODO: When/where to delete this thing?
    PrimeFinder* pFndr = new PrimeFinder( tran, args );
    pFndr->run();
}

          
