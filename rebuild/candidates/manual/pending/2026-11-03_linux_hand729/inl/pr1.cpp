#include "engine/CCountedPointer.h"
struct X { virtual ~X(); };
X* Make(int);
bool Cond(int);
void Use(CCountedPointer<X>&);
void F(int n)
{
    CCountedPointer<X> p;
    if (Cond(0)) { p.Reset(Make(0)); Use(p); }
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            if (Cond(1)) {
                if (Cond(2)) { p.Reset(Make(2)); Use(p); }
                else if (Cond(3)) { p.Reset(Make(3)); Use(p); }
                else if (Cond(4)) { p.Reset(Make(4)); Use(p); }
            }
    for (int k = 0; k < n; ++k)
        if (Cond(5)) {
            if (Cond(6)) p.Reset(Make(6));
            else if (Cond(7)) p.Reset(Make(7));
            else if (Cond(8)) p.Reset(Make(8));
            else p.Reset(Make(9));
            Use(p);
        }
    for (int m = 0; m < n; ++m)
        if (Cond(10)) {
            if (Cond(11)) { p.Reset(Make(11)); Use(p); }
        }
}
