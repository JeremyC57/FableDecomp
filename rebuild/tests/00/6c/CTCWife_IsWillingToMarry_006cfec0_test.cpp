#include <cstdio>
class CThing { public: unsigned char m_flags1c9; };
class CThingBuilding { public: int id; };
static CThingBuilding g_home;
class CTCWife { public:
    CThing* ParentThing; bool courting; int OpinionAttitude; bool ReceivedWeddingRing; bool hasHome;
    bool IsCourtingHero() const { return courting; }
    bool IsInLoveWithHero() const { return IsCourtingHero() && OpinionAttitude == 12; }
    CThingBuilding* FindMaritalHome() const { return hasHome ? &g_home : 0; }
    bool IsWillingToMarry() const
    {
        if (IsCourtingHero() && IsInLoveWithHero() && (ParentThing->m_flags1c9 & 0x10) && ReceivedWeddingRing && FindMaritalHome()) return true;
        return false;
    }
};
int main()
{
    CThing t; t.m_flags1c9 = 0x10; CTCWife w; w.ParentThing = &t; w.courting = true; w.OpinionAttitude = 12; w.ReceivedWeddingRing = true; w.hasHome = true;
    if (!w.IsWillingToMarry()) { std::printf("006cfec0_TEST FAIL\n"); return 1; }
    w.hasHome = false; if (w.IsWillingToMarry()) { std::printf("006cfec0_TEST FAIL\n"); return 1; }
    w.hasHome = true; w.ReceivedWeddingRing = false; if (w.IsWillingToMarry()) { std::printf("006cfec0_TEST FAIL\n"); return 1; }
    w.ReceivedWeddingRing = true; t.m_flags1c9 = 0xef; if (w.IsWillingToMarry()) { std::printf("006cfec0_TEST FAIL\n"); return 1; }
    t.m_flags1c9 = 0x10; w.OpinionAttitude = 1; if (w.IsWillingToMarry()) { std::printf("006cfec0_TEST FAIL\n"); return 1; }
    w.OpinionAttitude = 12; w.courting = false; if (w.IsWillingToMarry()) { std::printf("006cfec0_TEST FAIL\n"); return 1; }
    std::printf("006cfec0_TEST PASS\n");
    return 0;
}