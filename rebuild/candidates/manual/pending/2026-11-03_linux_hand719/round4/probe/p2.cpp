#define FABLE_STLP_OUTOFLINE_UNINIT_COPY
#include "stlport/fable_containers.h"
class C2DLineF { public: float a,b,c,d; };
class CSwitchableBlockage { public: unsigned __int64 UniqueID; std::list<C2DLineF> LineList; };
struct L { std::vector<C2DLineF> a; std::vector<CSwitchableBlockage> SwitchableBlockages; };
void fill(CSwitchableBlockage&, long);
void probe(std::vector<L>& v, long n)
{
    for (long i = 0; i < n; ++i) {
        CSwitchableBlockage b;
        fill(b, i);
        v[i].SwitchableBlockages.push_back(b);
    }
}
