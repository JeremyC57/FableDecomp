#include <cstdio>
struct CAIBrain {}; struct CThingAICreature {}; struct CCharString {};
static int g_baseCalls; static long g_basePriority;
struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase() {}
    virtual void Initialise(CAIBrain&, CThingAICreature&, long priority, long, const CCharString&) { ++g_baseCalls; g_basePriority = priority; }
    unsigned char _pad[0x10];
};
struct CAIStateGroup_BanditSitRoundCampfire : CAIStateGroupBase {
    unsigned char PCampfire[8]; long SeatPosIndex; bool GotSeatPos; bool Sitting;
    unsigned long FrameToNextSitOnSeat; unsigned long FrameToNextCheckIsRunnable; bool WasCarryingWeapon;
    virtual void Initialise(CAIBrain& b, CThingAICreature& c, long p, long sp, const CCharString& n)
    { CAIStateGroupBase::Initialise(b, c, p, sp, n); FrameToNextSitOnSeat = 0; WasCarryingWeapon = false; FrameToNextCheckIsRunnable = 0; }
};
int main()
{
    CAIStateGroup_BanditSitRoundCampfire g; CAIBrain b; CThingAICreature c; CCharString n;
    g.FrameToNextSitOnSeat = 77; g.FrameToNextCheckIsRunnable = 88; g.WasCarryingWeapon = true; g.SeatPosIndex = 5;
    g.Initialise(b, c, 42, 3, n);
    if (g_baseCalls != 1 || g_basePriority != 42 || g.FrameToNextSitOnSeat != 0 || g.FrameToNextCheckIsRunnable != 0 || g.WasCarryingWeapon || g.SeatPosIndex != 5) { std::printf("0093a500_TEST FAIL\n"); return 1; }
    std::printf("0093a500_TEST PASS\n"); return 0;
}