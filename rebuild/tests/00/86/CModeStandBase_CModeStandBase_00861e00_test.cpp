// Test for 00861e00 CModeStandBase::CModeStandBase: base mode ctor, zeroed counters/speeds, empty name and anim pointers.
#include <cstdio>
class CThing { public: int id; };
static int g_baseCalls = 0;
struct CModeBase {
    CThing* ParentThing;
    CModeBase(CThing* parent) : ParentThing(parent) { ++g_baseCalls; }
    virtual ~CModeBase() {}
};
class CCharString { public: const char* PStringData; CCharString() : PStringData("") {} };
struct CAnimEntryPointer { void* Data; void* Info; CAnimEntryPointer() : Data(0), Info(0) {} };
struct CModeStandBase : CModeBase {
    long IdleCounter; long FramesToKeepUpdatingMovement; bool ForceAddNewAnims;
    float MaxRunSpeed, MaxJogSpeed, MaxWalkSpeed;
    CCharString PoseAnimName;
    CAnimEntryPointer PStandAnimation, PSlowWalkAnimation, PWalkAnimation, PJogAnimation, PRunAnimation, PSprintAnimation;
    CModeStandBase(CThing* parent)
        : CModeBase(parent), IdleCounter(0), FramesToKeepUpdatingMovement(0), ForceAddNewAnims(false),
          MaxRunSpeed(0.0f), MaxJogSpeed(0.0f), MaxWalkSpeed(0.0f) {}
};
int main()
{
    CThing t = { 1 };
    CModeStandBase m(&t);
    if (g_baseCalls != 1 || m.ParentThing != &t || m.IdleCounter != 0 || m.FramesToKeepUpdatingMovement != 0
        || m.ForceAddNewAnims || m.MaxRunSpeed != 0.0f || m.MaxJogSpeed != 0.0f || m.MaxWalkSpeed != 0.0f
        || m.PoseAnimName.PStringData[0] != 0 || m.PStandAnimation.Data != 0 || m.PSprintAnimation.Info != 0
        || m.PRunAnimation.Data != 0) { std::printf("00861e00_TEST FAIL\n"); return 1; }
    std::printf("00861e00_TEST PASS\n");
    return 0;
}