#define FABLE_STLP_OUTOFLINE_UNINIT_COPY
#define FABLE_STLP_BACK_INSERTER
#include "stlport/fable_containers.h"
class C2DLineF { public: float a,b,c,d; };
class CSwitchableBlockage { public: unsigned __int64 UniqueID; std::list<C2DLineF> LineList; };
struct L { std::vector<C2DLineF> a; std::vector<CSwitchableBlockage> SwitchableBlockages; };
void fill(std::vector<C2DLineF>&, long);
void probe(std::vector<L>& v, std::list<long>& things)
{
    for (std::list<long>::iterator it = things.begin(); it != things.end(); ++it) {
      long n = *it;
      if (n > 3) {
        long i = 0; while (i < n) {
            CSwitchableBlockage b;
            b.UniqueID = n;
            std::vector<C2DLineF> lines;
            fill(lines, i);
            std::copy(lines.begin(), lines.end(), std::back_inserter(b.LineList));
            v[i].SwitchableBlockages.push_back(b);
            ++i;
        }
      }
    }
}
