// Behaviour test for 006e8b60 CCombatRing::Reset: the ring thickness is zeroed and every prong's
// occupier pointer is cleared; other prong data is untouched.
#include <cstdio>

struct CPtr { void* item; CPtr(void* i) : item(i) { } CPtr& operator=(const CPtr& o) { item = o.item; return *this; } };
struct CProng { bool Blocked; long Timer; CPtr POccupier; CProng() : Blocked(true), Timer(5), POccupier(0) { } };
struct CRing {
    float RingThickness; CProng* first; CProng* last;
    void Reset() { RingThickness = 0.0f; for (CProng* p = first; p != last; ++p) p->POccupier = CPtr(0); }
};

int main()
{
    int a, b; CProng prongs[3];
    prongs[0].POccupier = CPtr(&a); prongs[2].POccupier = CPtr(&b);
    CRing r; r.RingThickness = 2.5f; r.first = prongs; r.last = prongs + 3;
    r.Reset();
    bool ok = r.RingThickness == 0.0f && prongs[0].POccupier.item == 0 && prongs[2].POccupier.item == 0 && prongs[1].Timer == 5 && prongs[0].Blocked;
    if (!ok) { std::printf("006e8b60_TEST FAIL\n"); return 1; }
    std::printf("006e8b60_TEST PASS\n");
    return 0;
}
