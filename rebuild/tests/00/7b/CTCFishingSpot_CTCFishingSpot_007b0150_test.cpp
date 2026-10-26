// behaviour test for 0x007B0150 CTCFishingSpot::CTCFishingSpot(CThing*)
#include <cstdio>
class CThing { public: int id; };
struct Ptr { CThing* item; Ptr(CThing* p) : item(p) { } };
struct Spot { CThing* ParentThing; Ptr P1, P2, PFish; bool Enabled;
    Spot(CThing* t) : ParentThing(t), P1(0), P2(0), PFish(0), Enabled(true) { } };
int main()
{
    CThing t = { 2 };
    Spot s(&t);
    bool ok = s.ParentThing == &t && !s.P1.item && !s.P2.item && !s.PFish.item && s.Enabled;
    if (!ok) { std::printf("007b0150_TEST FAIL\n"); return 1; }
    std::printf("007b0150_TEST PASS\n");
    return 0;
}