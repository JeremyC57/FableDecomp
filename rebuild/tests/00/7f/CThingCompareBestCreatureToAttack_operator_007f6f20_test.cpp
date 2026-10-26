// CThingCompare_BestCreatureToAttack::operator()(CThingCreatureBase* a, CThingCreatureBase* b) -- the <CThingCreatureBase> instantiation: pick the better attack target of two: none when neither is alive, the live one when only one is, else the one with the higher compare value
// Leaf: ??R
// Retail: Fable.exe (TLC, VC7.1) @ 0x007F6F20, 110 bytes (`ret 8`).
// Callees: CThingCompare_BestCreatureToAttack::GetCompareValueForThing<CThingCreatureBase>(CThingCreatureBase*) @ 0x007F6BF0 (float).
// Identity: a template instantiation of the comparator used by GetPBestThing searches; the manifest label operator()<CThingCreatureBase> is kept (caller 0x007F65B9).
// Layout: retail CThing Dead flag = +0x91 bit 0.
// Shape: nested form with separate returns (`return 0` last, `return b` shared by the dead-a and lost-compare arms);
//   IS_THING_ALIVE is a macro `((p) != 0 && !(p)->IsDead())` over an inline CThing::IsDead() (as an inline function VC
//   lays `return 0` inline after the first alive test); the bit mask is kept in al; the second test re-checks a; b is
//   only null-tested before the compare; `valueA > valueB` with valueA spilled (`fcomp ; test ah,5 ; jp`).
class CThing { public:
    unsigned char _pad[0x91];
    unsigned char Flags91;                      // +0x91 bit 0 dead
    bool IsDead() const { return (Flags91 & 1) != 0; }
};
class CThingCreatureBase : public CThing { public: };
class CThingAICreature : public CThingCreatureBase { public: };
#define IS_THING_ALIVE(p) ((p) != 0 && !(p)->IsDead())

class CThingCompare_BestCreatureToAttack { public:
    template<class T> float GetCompareValueForThing(T* thing) const;    // 0x007F6BF0 for CThingCreatureBase
    template<class T> T* operator()(T* a, T* b) const
    {
        if (IS_THING_ALIVE(a) || IS_THING_ALIVE(b)) {
            if (IS_THING_ALIVE(a)) {
                if (b) {
                    float valueA = GetCompareValueForThing(a);
                    if (!(valueA > GetCompareValueForThing(b)))
                        return b;
                }
                return a;
            }
            return b;
        }
        return 0;
    }
};

template CThingCreatureBase* CThingCompare_BestCreatureToAttack::operator()(CThingCreatureBase* a, CThingCreatureBase* b) const;

// ---- test harness ----
#include <stdio.h>
#include <string.h>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
static int g_calls = 0;
template<class T> float CThingCompare_BestCreatureToAttack::GetCompareValueForThing(T* thing) const
{ ++g_calls; return (float)thing->_pad[0]; }

int main()
{
    CThingCompare_BestCreatureToAttack cmp;
    static CThingCreatureBase a, b;
    memset(&a, 0, sizeof(a)); memset(&b, 0, sizeof(b));
    a._pad[0] = 5; b._pad[0] = 3;
    CThingCreatureBase* n = 0;
    CHECK(cmp(n, n) == 0);
    CHECK(cmp(&a, n) == &a && g_calls == 0);
    CHECK(cmp(n, &b) == &b && g_calls == 0);
    CHECK(cmp(&a, &b) == &a && g_calls == 2);
    b._pad[0] = 9;
    CHECK(cmp(&a, &b) == &b && g_calls == 4);
    b._pad[0] = 5;                                   // tie: b
    CHECK(cmp(&a, &b) == &b && g_calls == 6);
    a.Flags91 = 0x81;                                // a dead
    CHECK(cmp(&a, &b) == &b && g_calls == 6);
    b.Flags91 = 1;                                   // both dead
    CHECK(cmp(&a, &b) == 0);
    a.Flags91 = 0xfe; b._pad[0] = 3;                 // a alive (other bits), b dead but non-null: still compared
    CHECK(cmp(&a, &b) == &a && g_calls == 8);
    CHECK(cmp(&a, n) == &a && g_calls == 8);
    if (fails == 0) printf("007f6f20_TEST PASS\n");
    return fails;
}