#include <stdio.h>
#define FABLE_RETAIL_ASSERT(x) ((void)0)

struct C3DVector { float x, y, z; };
struct CCharString { int Name; };
struct CAnimationEntry { int Animation; };
struct CAnimFilterAnyGroup {};
const CCharString SIT_ON_CHAIR_ANIM_NAME = { 31 };
const CCharString SIT_ON_BENCH_ANIM_NAME = { 32 };
const unsigned long THING_OBJECT_PROPERTY_CHAIR = 0x10000;
const unsigned long THING_OBJECT_PROPERTY_BENCH = 0x20000;
const int AI_ACTION_FINISHED = 2;
int AliveModes;
struct CThingCreatureBase {
    int RequestedAnimation;
    CAnimationEntry Animation;
    const CAnimationEntry& GetRandomAnimFromNameWithFilter(const CCharString& name, const CAnimFilterAnyGroup&) {
        RequestedAnimation = name.Name;
        Animation.Animation = name.Name;
        return Animation;
    }
};
struct Seat {
    unsigned long Properties;
    bool HasProperty(unsigned long property) const { return (Properties & property) != 0; }
};
namespace NCreatureMode {
struct CModeSitBase {
    CModeSitBase(bool bench, CThingCreatureBase& creature) : Bench(bench), Creature(creature) { ++AliveModes; }
    virtual ~CModeSitBase() { --AliveModes; }
    bool Bench;
    CThingCreatureBase& Creature;
};
struct CModeSitBench : CModeSitBase {
    CModeSitBench(CThingCreatureBase& creature) : CModeSitBase(true, creature) {}
};
}
template<typename T> struct CCountedPointer {
    CCountedPointer() : P(0) {}
    ~CCountedPointer() { delete P; }
    void Reset(T* value) { delete P; P = value; }
    T* P;
private:
    CCountedPointer(const CCountedPointer&);
    CCountedPointer& operator=(const CCountedPointer&);
};
struct CCreatureAction_SitDown {
    CCreatureAction_SitDown(CThingCreatureBase& creature, const CAnimationEntry& animation,
                            const CCountedPointer<NCreatureMode::CModeSitBase>& mode)
        : Creature(creature), Animation(animation), Mode(*mode.P) {}
    CThingCreatureBase& Creature;
    const CAnimationEntry& Animation;
    const NCreatureMode::CModeSitBase& Mode;
};
class CAIStateGroup_UsesSeat {
public:
    bool SitInSeat();
    CThingCreatureBase& GetCreature() { return Creature; }
    NCreatureMode::CModeSitBase* AllocChairSittingMode() {
        ++ChairAllocations;
        return new NCreatureMode::CModeSitBase(false, Creature);
    }
    bool IsStopping() const { return Stopping; }
    int AIAction_PerformAction(const CCreatureAction_SitDown& action) {
        ++Actions;
        ValidAction = Sitting && AliveModes == 1 && (&Creature == &action.Creature)
            && (&Creature == &action.Mode.Creature) && action.Mode.Bench == SittingOnBench
            && action.Animation.Animation == Creature.RequestedAnimation;
        return ActionResult;
    }
    CThingCreatureBase Creature;
    Seat* PSeat;
    C3DVector SeatPos;
    bool SittingOnBench;
    bool Sitting;
    bool Stopping;
    int ChairAllocations;
    int Actions;
    int ActionResult;
    bool ValidAction;
};

bool CAIStateGroup_UsesSeat::SitInSeat()
{
    FABLE_RETAIL_ASSERT(GFWorld2DDistanceSmallerThan(GetCreature().GetPos(), SeatPos, 0.1f, false));
    CAnimationEntry anim_entry;
    CCountedPointer<NCreatureMode::CModeSitBase> psit_mode;
    if (PSeat->HasProperty(THING_OBJECT_PROPERTY_CHAIR)) {
        FABLE_RETAIL_ASSERT(!PSeat->HasProperty(THING_OBJECT_PROPERTY_BENCH));
        anim_entry = GetCreature().GetRandomAnimFromNameWithFilter(SIT_ON_CHAIR_ANIM_NAME, CAnimFilterAnyGroup());
        NCreatureMode::CModeSitBase* pmode = AllocChairSittingMode();
        FABLE_RETAIL_ASSERT(pmode);
        psit_mode.Reset(pmode);
        SittingOnBench = false;
    } else {
        FABLE_RETAIL_ASSERT(PSeat->HasProperty(THING_OBJECT_PROPERTY_BENCH));
        anim_entry = GetCreature().GetRandomAnimFromNameWithFilter(SIT_ON_BENCH_ANIM_NAME, CAnimFilterAnyGroup());
        psit_mode.Reset(new NCreatureMode::CModeSitBench(GetCreature()));
        SittingOnBench = true;
    }
    if (IsStopping())
        return false;
    Sitting = true;
    if (AIAction_PerformAction(CCreatureAction_SitDown(GetCreature(), anim_entry, psit_mode)) != AI_ACTION_FINISHED)
        return false;
    return true;
}

int main() {
    int cases = 0;
    for (int bench = 0; bench != 2; ++bench) {
        for (int stopping = 0; stopping != 2; ++stopping) {
            for (int result = 0; result != 4; ++result) {
                Seat seat = { bench ? THING_OBJECT_PROPERTY_BENCH : THING_OBJECT_PROPERTY_CHAIR };
                CAIStateGroup_UsesSeat group = {};
                group.PSeat = &seat;
                group.Stopping = stopping != 0;
                group.SittingOnBench = bench == 0;
                group.ActionResult = result;
                const bool expected = !stopping && result == AI_ACTION_FINISHED;
                const bool actual = group.SitInSeat();
                const int expectedAnim = bench ? SIT_ON_BENCH_ANIM_NAME.Name : SIT_ON_CHAIR_ANIM_NAME.Name;
                if (actual != expected || group.SittingOnBench != (bench != 0)
                    || group.Sitting != !stopping || group.ChairAllocations != !bench
                    || group.Actions != !stopping || group.ValidAction != !stopping
                    || group.Creature.RequestedAnimation != expectedAnim || AliveModes != 0) {
                    printf("FAIL bench=%d stopping=%d action=%d\n", bench, stopping, result);
                    return 1;
                }
                ++cases;
            }
        }
    }
    if (cases != 16)
        return 2;
    puts("00938380_TEST PASS");
    return 0;
}