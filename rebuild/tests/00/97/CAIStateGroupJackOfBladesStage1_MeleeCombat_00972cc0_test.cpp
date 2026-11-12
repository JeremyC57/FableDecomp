#include <cstdio>
#include <cmath>
// Behaviour model of MeleeCombat: loop logic with stubbed world.
struct Vec { float X, Y, Z; };
enum EMovementType { AI_MOVEMENT_NULL = 0 };
// Model of CBrainOutput's inline members used by the row (shared engine/campfire_types.h opt-in FABLE_RETAIL_CBRAINOUTPUT_INLINE).
struct NavigationInput { Vec Destination; float Proximity; EMovementType MovementType; float Speed;
    NavigationInput() : Proximity(0.0f), MovementType(AI_MOVEMENT_NULL), Speed(0.0f) { } };
struct BrainOutput { unsigned long Types; long LookFlags; NavigationInput Nav;
    BrainOutput() : Types(0), LookFlags(0) { }
    void SetAsPumpCombatActions() { Types |= 0x20; }
    void SetAsLookAtThing(long flags) { Types |= 4; LookFlags = flags; } };
struct Thing { bool dead; Vec pos; };
struct Sim {
    Thing* target; Thing* arena; Thing me; bool stopping; bool inMelee; bool readyCalled;
    float limit; int rushAfter; int ticks; int outputs; bool terminated; int lastFlags; unsigned lastTypes;
protected:
    bool IsStillRunnable() { return target && !target->dead && !stopping && arena && !arena->dead; }
    bool HasHitsCausedRush() { return ticks >= rushAfter; }
    void Yield() { ++ticks; if (ticks == 2) target->pos.X += 1.0f; }
public:
    int MeleeCombat()
    {
        if (inMelee) readyCalled = true;
        for (;;) {
            if (!IsStillRunnable()) { terminated = true; return 2; }
            float distance;
            {
                BrainOutput output; output.SetAsPumpCombatActions(); output.SetAsLookAtThing(0x11);
                if (output.Nav.MovementType != AI_MOVEMENT_NULL) return -1;
                lastTypes = output.Types; lastFlags = output.LookFlags; ++outputs;
                Yield();
                float dx = target->pos.X - me.pos.X, dy = target->pos.Y - me.pos.Y;
                distance = (float)std::sqrt(dy * dy + dx * dx);
            }
            if (!IsStillRunnable()) return 2;
            if (!(distance < limit)) return 2;
            if (HasHitsCausedRush()) return 2;
        }
    }
};
int main()
{
    Thing t = { false, { 3.0f, 4.0f, 0.0f } }, a = { false, { 0, 0, 0 } };
    Sim s = { &t, &a, { false, { 0, 0, 0 } }, false, true, false, 5.5f, 100, 0, 0, false, 0, 0 };
    int r = s.MeleeCombat();
    // distance 5 < 5.5 on tick 1; tick 2 moves target to (4,4): 5.66 >= 5.5 -> leave without terminate
    if (r != 2 || s.ticks != 2 || s.terminated || !s.readyCalled || s.lastTypes != 0x24 || s.lastFlags != 0x11) { std::printf("00972cc0_TEST FAIL\n"); return 1; }
    Thing d = { true, { 0, 0, 0 } };
    Sim s2 = { &d, &a, { false, { 0, 0, 0 } }, false, false, false, 5.5f, 100, 0, 0, false, 0, 0 };
    if (s2.MeleeCombat() != 2 || !s2.terminated || s2.outputs != 0 || s2.readyCalled) { std::printf("00972cc0_TEST FAIL\n"); return 1; }
    Thing t3 = { false, { 1.0f, 0.0f, 0.0f } };
    Sim s3 = { &t3, &a, { false, { 0, 0, 0 } }, false, false, false, 5.5f, 3, 0, 0, false, 0, 0 };
    if (s3.MeleeCombat() != 2 || s3.ticks != 3 || s3.terminated) { std::printf("00972cc0_TEST FAIL\n"); return 1; }
    std::printf("00972cc0_TEST PASS\n");
    return 0;
}