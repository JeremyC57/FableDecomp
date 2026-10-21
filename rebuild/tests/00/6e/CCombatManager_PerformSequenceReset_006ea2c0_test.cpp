// Behaviour test for CCombatManager::PerformSequenceReset (0x006EA2C0): a running melee sequence advances the
// victim's pattern and may start the ring rotating (chance roll, coin for the direction); the sequence is stopped
// and the index cleared; idle sequences are only cleared.
#include <cstdio>

static unsigned long GFROR13(unsigned long v) { return (v >> 13) | (v << 19); }
static unsigned long WorldRandom(unsigned long& seed) { seed = seed * 0x24A1 + 0x24DF; seed = GFROR13(seed); return seed; }
struct CRingRotationDef { long RotationChance; float RotationSpeed; long RefCount; };
struct CCombatTypeDef { CRingRotationDef* Rotation; bool GetSubDef(CRingRotationDef*& out) const { out = Rotation; if (out) ++out->RefCount; return out != 0; } };
struct CThingCreatureBase { int Id; CCombatTypeDef* Type; bool OnRotatingRing; CCombatTypeDef* GetCombatTypeDef() const { return Type; } };
struct CCombatSequenceBase { bool Melee, Running; int Stops; bool IsMeleeSequence() { return Melee; } void Stop() { ++Stops; Running = false; } };
struct CCombatantInfo { CThingCreatureBase* Parent; CThingCreatureBase* Attacked; long CurrentSequence; CCombatSequenceBase* Sequences[1]; int PatternSteps; };
struct CCombatManager {
    CCombatantInfo* Infos; int Count; unsigned long Seed; int Rotations; float LastSpeed;
    CCombatantInfo& GetCombatantInfo(const CThingCreatureBase& c) { for (int i = 0; i < Count; ++i) if (Infos[i].Parent->Id == c.Id) return Infos[i]; return Infos[0]; }
    void IncrementDefenderAttackPattern(CCombatantInfo& info) { ++info.PatternSteps; }
    bool IsCreatureOnRotatingCombatRing(const CThingCreatureBase& c) { return c.OnRotatingRing; }
    void SetAttackerRingAsRotating(const CThingCreatureBase&, float s) { ++Rotations; LastSpeed = s; }
    void PerformSequenceReset(CThingCreatureBase& creature);
};

void CCombatManager::PerformSequenceReset(CThingCreatureBase& creature)
{
    CCombatantInfo& info = GetCombatantInfo(creature);
    CCombatantInfo* victim = &GetCombatantInfo(*info.Attacked);
    if (info.CurrentSequence >= 0) {
        CCombatSequenceBase* sequence = info.Sequences[info.CurrentSequence];
        if (sequence->Running) {
            if (sequence->IsMeleeSequence()) {
                IncrementDefenderAttackPattern(*victim);
                CRingRotationDef* rotation = 0;
                if (!IsCreatureOnRotatingCombatRing(creature)) {
                    if (creature.GetCombatTypeDef()->GetSubDef(rotation)) {
                        long roll = WorldRandom(Seed) % 100;
                        if (roll <= rotation->RotationChance) {
                            float speed = rotation->RotationSpeed;
                            float direction = (WorldRandom(Seed) & 1) == 0 ? 1.0f : -1.0f;
                            SetAttackerRingAsRotating(creature, direction * speed);
                        }
                    }
                }
                if (rotation) --rotation->RefCount;
            }
            sequence->Stop();
        }
        info.CurrentSequence = -1;
    }
}

int main()
{
    CRingRotationDef rot = { 100, 30.0f, 1 };           // always rotates
    CCombatTypeDef type = { &rot };
    CThingCreatureBase a = { 1, &type, false }, v = { 2, &type, false };
    CCombatSequenceBase melee = { true, true, 0 };
    CCombatantInfo infos[2] = { { &a, &v, 0, { &melee }, 0 }, { &v, &a, -1, { 0 }, 0 } };
    CCombatManager m; m.Infos = infos; m.Count = 2; m.Seed = 7; m.Rotations = 0; m.LastSpeed = 0.0f;
    m.PerformSequenceReset(a);
    bool ok = infos[1].PatternSteps == 1 && m.Rotations == 1 && (m.LastSpeed == 30.0f || m.LastSpeed == -30.0f)
        && melee.Stops == 1 && infos[0].CurrentSequence == -1 && rot.RefCount == 1;
    m.PerformSequenceReset(a);                          // no current sequence: nothing
    ok = ok && melee.Stops == 1 && infos[1].PatternSteps == 1;
    melee.Running = true; infos[0].CurrentSequence = 0; a.OnRotatingRing = true;
    m.PerformSequenceReset(a);                          // already rotating: no new rotation
    ok = ok && m.Rotations == 1 && infos[1].PatternSteps == 2 && melee.Stops == 2;
    if (!ok) { std::printf("006ea2c0_TEST FAIL\n"); return 1; }
    std::printf("006ea2c0_TEST PASS\n");
    return 0;
}