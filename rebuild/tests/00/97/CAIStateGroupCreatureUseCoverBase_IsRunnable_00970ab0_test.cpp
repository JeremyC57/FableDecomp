// behaviour test for CAIStateGroup_CreatureUseCoverBase::IsRunnable (0x00970AB0)
#include <cstdio>
struct CThing { int id; };
struct CTC8 { CThing* target; CThing* GetTarget() { return target; } };
struct Creature { unsigned int bits[9]; CTC8* tc8; bool HasTC(long k) const { return (bits[k >> 5] & (1u << (k & 31))) != 0; } };
static bool attackOk;
static bool IsToAttackTarget(Creature*, CThing* t) { return attackOk && t != 0; }
struct Group {
    Creature* creature; CThing* PTargetThing; bool leaf; int leafCalls;
    bool IsRunnableLeaf() { ++leafCalls; return leaf; }
    bool IsRunnable()
    {
        if (creature->HasTC(8)) {
            CThing* target = creature->tc8->GetTarget();
            if (target && IsToAttackTarget(creature, target)) {
                PTargetThing = target;
                return IsRunnableLeaf();
            }
        }
        return false;
    }
};
int main()
{
    CThing t = { 1 }; CTC8 tc = { &t }; Creature c = { {0}, &tc };
    Group g = { &c, 0, true, 0 };
    attackOk = true;
    bool r1 = g.IsRunnable();                       // no component
    c.bits[0] = 0x100;
    bool r2 = g.IsRunnable();
    attackOk = false; g.PTargetThing = 0;
    bool r3 = g.IsRunnable();
    if (r1 || !r2 || r3 || g.leafCalls != 1 || g.PTargetThing != 0) { std::printf("00970ab0_TEST FAIL\n"); return 1; }
    std::printf("00970ab0_TEST PASS\n");
    return 0;
}