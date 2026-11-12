// Behaviour test for 0x0092E130 CAIStateGroup_MinionSleep::IsRunnable(): the row's logic over stub classes (C++03, no Windows headers).
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string>
static std::string trace;
static void check(bool v, int line) { if (!v) { printf("0092e130_TEST FAIL line %d\n", line); exit(1); } }
class C3DVector { public: float X, Y, Z; };
class C2DVector { public: float X, Y; C2DVector(float x, float y) : X(x), Y(y) { } float GetMagnitude() const { return (float)sqrt(X * X + Y * Y); } };
class CRightHandedSet { public: C3DVector Up, Forward; const C3DVector& GetForward() const { return Forward; } };
class CTCPhysicsBase { public: CRightHandedSet rh; const CRightHandedSet& GetRHSet() const { trace += 'R'; return rh; } };
class CTCSleep { public:
    bool ready, fast;
    bool GetFastTrack() { trace += 'F'; return fast; }
    bool GetIsReadyToSleep() { return ready; }
};
class CThingAICreature { public:
    CTCSleep* sleep; C3DVector pos; CTCPhysicsBase physics;
    template <class T> bool GetTC(T** out) { trace += 'T'; if (!sleep) return false; *out = sleep; return true; }
    const C3DVector& GetPos() const { trace += 'P'; return pos; }
    CTCPhysicsBase* GetPhysicsTC() { return &physics; }
};
class CAIStateGroup_GoToBed { public:
    CThingAICreature creature; bool base_runnable; C3DVector BedPos; CRightHandedSet Rhset;
    CThingAICreature& GetCreature() const { return const_cast<CThingAICreature&>(creature); }
    virtual bool IsRunnable() { trace += 'B'; return base_runnable; }
    virtual ~CAIStateGroup_GoToBed() { }
};
class CAIStateGroup_MinionSleep : public CAIStateGroup_GoToBed { public: virtual bool IsRunnable(); };

bool CAIStateGroup_MinionSleep::IsRunnable()
{
    CTCSleep* sleep;
    if (!GetCreature().GetTC(&sleep))
        return false;
    if (!sleep->GetFastTrack()) {
        if (!sleep->GetIsReadyToSleep())
            return false;
        if (!CAIStateGroup_GoToBed::IsRunnable())
            return false;
        const C3DVector& pos = GetCreature().GetPos();
        if (C2DVector(BedPos.X - pos.X, BedPos.Y - pos.Y).GetMagnitude() > 0.01f)
            return false;
        const C3DVector& facing = GetCreature().GetPhysicsTC()->GetRHSet().GetForward();
        if (C2DVector(Rhset.GetForward().X - facing.X, Rhset.GetForward().Y - facing.Y).GetMagnitude() > 0.1f)
            return false;
    }
    return true;
}

int main()
{
    const float offsets[] = { 0.0f, 0.005f, 0.05f, 0.5f, 3.0f };
    for (int has = 0; has != 2; ++has)
    for (int bits = 0; bits != 8; ++bits)
    for (int p = 0; p != 5; ++p)
    for (int f = 0; f != 5; ++f) {
        CTCSleep sleep; sleep.fast = (bits & 1) != 0; sleep.ready = (bits & 2) != 0;
        CAIStateGroup_MinionSleep group;
        group.creature.sleep = has ? &sleep : 0;
        group.base_runnable = (bits & 4) != 0;
        group.BedPos.X = 10.0f; group.BedPos.Y = 20.0f; group.BedPos.Z = 99.0f;
        group.creature.pos.X = 10.0f + offsets[p]; group.creature.pos.Y = 20.0f; group.creature.pos.Z = -5.0f;
        group.Rhset.Forward.X = 0.0f; group.Rhset.Forward.Y = 1.0f; group.Rhset.Forward.Z = 0.0f;
        group.creature.physics.rh.Forward.X = 0.0f; group.creature.physics.rh.Forward.Y = 1.0f - offsets[f]; group.creature.physics.rh.Forward.Z = 7.0f;
        trace = "";
        bool got = group.IsRunnable();
        std::string expected = "T"; bool want = false;
        if (has) {
            expected += 'F';
            if (sleep.fast) want = true;
            else if (sleep.ready) {
                expected += 'B';
                if (group.base_runnable) {
                    expected += 'P';
                    if (offsets[p] <= 0.01f) { expected += 'R'; want = offsets[f] <= 0.1f; }
                }
            }
        }
        check(got == want, __LINE__);
        check(trace == expected, __LINE__);
    }
    puts("0092e130_TEST PASS");
    return 0;
}