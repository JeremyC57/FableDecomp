// Behaviour test for 0x005BE284 NInventory::CTCInventoryBase::SetNumberOfSelectedItemOwned(long): a model; checks the defaults (max 1, none owned), the category per-slot maximum, and the count summed over the live hero three inventory components.
#include <cstdio>
#include <cmath>
#include <vector>
#include <string>
#include <set>
#include <map>

struct Inv { long n; long Count(long) const { return n; } };
struct Base { long Max, Num; };
static void Set(Base& b, long def, const long* catMax, bool heroAlive, const Inv* i11, const Inv* i13, const Inv* i12)
{
    b.Max = 1; b.Num = 0;
    if (def > 0) {
        if (catMax) b.Max = *catMax;
        long owned = 0;
        if (heroAlive) {
            if (i11) owned = i11->Count(def);
            if (i13) owned += i13->Count(def);
            if (i12) owned += i12->Count(def);
        }
        b.Num = owned;
    }
}
int main()
{
    int ok = 1; Base b; long m = 5; Inv x = { 2 }, y = { 3 }, z = { 4 };
    Set(b, 0, &m, true, &x, &y, &z); ok &= b.Max == 1 && b.Num == 0;
    Set(b, 7, &m, true, &x, &y, &z); ok &= b.Max == 5 && b.Num == 9;
    Set(b, 7, 0, true, 0, &y, 0); ok &= b.Max == 1 && b.Num == 3;
    Set(b, 7, &m, false, &x, &y, &z); ok &= b.Num == 0;
    std::printf(ok ? "005BE284_TEST PASS\n" : "005BE284_TEST FAIL\n"); return ok ? 0 : 1;
}