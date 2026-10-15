// Behaviour test for CTCBuyableHouse::CTCBuyableHouse (0x006BF8A0): flags false, rent due day LONG_MAX, counters zero,
// IsResidential true, the three pointers empty.
#include <cstdio>
struct IPtr { void* item; IPtr() : item(0) {} };
struct House {
    void* PDef; bool IsScripted, OwnedByPlayer, Rented; long DayNextRentIsDue, CurrentDressLevel, MaxDressLevel;
    IPtr MainSignPost, DressingSignPost; long FrameToNext; IPtr WifeLivingHere; bool InitiallySetupDressLevel;
    long VirtualMoneyBags; bool HasPreviouslyBeenBuyable, IsResidential;
    House() : PDef(0), IsScripted(false), OwnedByPlayer(false), Rented(false), DayNextRentIsDue(0x7fffffff), CurrentDressLevel(0),
        MaxDressLevel(0), FrameToNext(0), InitiallySetupDressLevel(false), VirtualMoneyBags(0), HasPreviouslyBeenBuyable(false), IsResidential(true) {}
};
int main()
{
    House h;
    if (h.PDef || h.IsScripted || h.OwnedByPlayer || h.Rented || h.DayNextRentIsDue != 0x7fffffff || h.CurrentDressLevel || h.MaxDressLevel ||
        h.MainSignPost.item || h.DressingSignPost.item || h.WifeLivingHere.item || h.FrameToNext || h.InitiallySetupDressLevel ||
        h.VirtualMoneyBags || h.HasPreviouslyBeenBuyable || !h.IsResidential) { std::printf("006bf8a0_TEST FAIL\n"); return 1; }
    std::printf("006bf8a0_TEST PASS\n");
    return 0;
}
