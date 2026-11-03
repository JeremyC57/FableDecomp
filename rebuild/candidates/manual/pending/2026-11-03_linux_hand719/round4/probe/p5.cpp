#define FABLE_STLP_OUTOFLINE_UNINIT_COPY
#include "stlport/fable_containers.h"
class C2DLineF { public: float a,b,c,d; };
class CSwitchableBlockage { public: unsigned __int64 UniqueID; std::list<C2DLineF> LineList; };
struct L { std::vector<C2DLineF> a; std::vector<CSwitchableBlockage> SwitchableBlockages; };
void fill(CSwitchableBlockage&, long);
void probe(std::vector<L>& v, std::list<long>& things)
{
    { std::vector<C2DLineF> w0; C2DLineF z; z.a=0; w0.push_back(z); w0.push_back(z); fill(*(CSwitchableBlockage*)&w0, 0); }
    { std::vector<C2DLineF> w1; C2DLineF z; z.a=1; w1.push_back(z); w1.push_back(z); fill(*(CSwitchableBlockage*)&w1, 0); }
    { std::vector<C2DLineF> w2; C2DLineF z; z.a=2; w2.push_back(z); w2.push_back(z); fill(*(CSwitchableBlockage*)&w2, 0); }
    { std::vector<C2DLineF> w3; C2DLineF z; z.a=3; w3.push_back(z); w3.push_back(z); fill(*(CSwitchableBlockage*)&w3, 0); }
    { std::vector<C2DLineF> w4; C2DLineF z; z.a=4; w4.push_back(z); w4.push_back(z); fill(*(CSwitchableBlockage*)&w4, 0); }
    { std::vector<C2DLineF> w5; C2DLineF z; z.a=5; w5.push_back(z); w5.push_back(z); fill(*(CSwitchableBlockage*)&w5, 0); }
    { std::vector<C2DLineF> w6; C2DLineF z; z.a=6; w6.push_back(z); w6.push_back(z); fill(*(CSwitchableBlockage*)&w6, 0); }
    { std::vector<C2DLineF> w7; C2DLineF z; z.a=7; w7.push_back(z); w7.push_back(z); fill(*(CSwitchableBlockage*)&w7, 0); }
    { std::vector<C2DLineF> w8; C2DLineF z; z.a=8; w8.push_back(z); w8.push_back(z); fill(*(CSwitchableBlockage*)&w8, 0); }
    { std::vector<C2DLineF> w9; C2DLineF z; z.a=9; w9.push_back(z); w9.push_back(z); fill(*(CSwitchableBlockage*)&w9, 0); }
    { std::vector<C2DLineF> w10; C2DLineF z; z.a=10; w10.push_back(z); w10.push_back(z); fill(*(CSwitchableBlockage*)&w10, 0); }
    { std::vector<C2DLineF> w11; C2DLineF z; z.a=11; w11.push_back(z); w11.push_back(z); fill(*(CSwitchableBlockage*)&w11, 0); }
    for (std::list<long>::iterator it = things.begin(); it != things.end(); ++it) {
      long n = *it;
      if (n > 3) {
        for (long i = 0; i < n; ++i) {
            CSwitchableBlockage b;
            fill(b, i);
            v[i].SwitchableBlockages.push_back(b);
        }
      }
    }
}
