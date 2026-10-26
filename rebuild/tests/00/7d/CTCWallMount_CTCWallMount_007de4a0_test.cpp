// Behaviour test for CTCWallMount::CTCWallMount @ 0x007DE4A0: bought for 0, no trophy, trophy id 0.
#include <cstdio>
class CThing { public: int id; };
class CTCBase { public: CThing* ParentThing; CTCBase(CThing* o) : ParentThing(o) {} };
struct CIntelligentPointer { void* p; CIntelligentPointer() : p(0) {} };
class CTCWallMount : public CTCBase { public:
    long BoughtForAmount; CIntelligentPointer Trophy; unsigned long TrophyIDLow, TrophyIDHigh;
    CTCWallMount(CThing* o) : CTCBase(o), BoughtForAmount(0), Trophy(), TrophyIDLow(0), TrophyIDHigh(0) {}
};
int main()
{
    CThing th; CTCWallMount w(&th);
    if (w.ParentThing != &th || w.BoughtForAmount || w.Trophy.p || w.TrophyIDLow || w.TrophyIDHigh) { std::printf("007de4a0_TEST FAIL\n"); return 1; }
    std::printf("007de4a0_TEST PASS\n");
    return 0;
}