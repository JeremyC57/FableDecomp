#include "engine/CCountedPointer.h"
struct X { virtual ~X(); };
X* Make(int);
bool Cond(int);
void Use(CCountedPointer<X>&);
struct S { ~S(); };
S Get(int);
void F(int n)
{
    CCountedPointer<X> p;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            S s = Get(0);
            if (Cond(1)) {
                if (Cond(2)) { p.Reset(Make(2)); }
                else if (Cond(3)) { p.Reset(Make(3)); }
                else if (Cond(4)) { p.Reset(Make(4)); }
                else continue;
                Use(p);
            }
        }
    }
    for (int k = 0; k < n; ++k)
        if (Cond(5)) {
            if (Cond(6)) p.Reset(Make(6));
            else if (Cond(7)) p.Reset(Make(7));
            else if (Cond(8)) p.Reset(Make(8));
            else p.Reset(Make(9));
            Use(p);
        }
}
