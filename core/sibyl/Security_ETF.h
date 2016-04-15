#ifndef SIBYL_SECURITY_ETF_
#define SIBYL_SECURITY_ETF_

#include <cmath>

#include "Security.h"

namespace sibyl
{

template <class TItem>
class ETF : public TItem // derive from a specialized Security<TOrder> (i.e., Item)
{
public:
    // virtuals from Security
    SecType Type  ()        const { return kSecETF;                     }
    INT     TckHi (INT p)   const { return p + 5;                       }
    INT     TckLo (INT p)   const { return (p - 5) * (p > 0);           }
    bool    ValidP(INT p)   const { return (p > 0) && ((p % 5) == 0);   }
    INT64   BFee  (INT64 r) const { return (INT64)std::round(r * dBF0); }
    INT64   SFee  (INT64 r) const { return (INT64)std::round(r * dSF0); }
    double  dBF   ()        const { return dBF0;                        }
    double  dSF   ()        const { return dSF0;                        }
    
    // additionals
    FLOAT   devNAV; // (NAV / price - 1) * 100
    
    ETF() : devNAV(0.0f) {}
private:
    constexpr static const double dBF0 = 0.00015;
    constexpr static const double dSF0 = 0.00015;
};

}

#endif /* SIBYL_SECURITY_ETF_ */