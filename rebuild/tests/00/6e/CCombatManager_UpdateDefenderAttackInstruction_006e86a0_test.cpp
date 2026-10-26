// Behaviour test for 0x006E86A0 CCombatManager::UpdateDefenderAttackInstruction.
#include <cstdio>
#include <cmath>
long g_framesPerSecond = 10;
long g_worldFrame = 100;
long GetWorldFrame() { return g_worldFrame; }
struct CCombatRing { float RotationAngle, DeltaRotation; };
struct CCombatWheel {
    CCombatRing* first; CCombatRing* last;
    CCombatRing* GetRing(long i) { if (i >= 0 && (unsigned long)i < (unsigned long)(last - first)) return &first[i]; return 0; }
};
struct CPattern { long* first; long* last; unsigned long size() const { return (unsigned long)(last - first); } long& operator[](unsigned long n) const { return *(first + n); } };
class CAttackPatternDef { public: CPattern AttackPattern; };
struct CCombatantInfo { CCombatWheel CombatWheel; CAttackPatternDef* PAttackPattern; long AttackPatternIndex; long FrameForNextInstruction; };
class CCombatManager { public:
    float AttackPatternIdleStandardInterval; int increments;
    void IncrementDefenderAttackPattern(CCombatantInfo& info)
    {
        ++increments;
        CPattern& pattern = info.PAttackPattern->AttackPattern;
        if ((unsigned long)++info.AttackPatternIndex >= pattern.size()) info.AttackPatternIndex = 0;
        long framesPerSecond = g_framesPerSecond;
        info.FrameForNextInstruction = (long)(GetWorldFrame() + framesPerSecond * AttackPatternIdleStandardInterval);
        if (pattern[info.AttackPatternIndex] > 0) {
            CCombatRing* ring = info.CombatWheel.GetRing(0);
            if (ring) ring->DeltaRotation = 0.0f / g_framesPerSecond;
        }
    }
    void UpdateDefenderAttackInstruction(CCombatantInfo& info)
    {
        CPattern& pattern = info.PAttackPattern->AttackPattern;
        long now = GetWorldFrame();
        if (pattern[info.AttackPatternIndex] != 0) {
            CCombatRing* ring = info.CombatWheel.GetRing(0);
            if (ring && std::fabs(ring->DeltaRotation) > 0.0001f) ring->DeltaRotation = 0.0f / g_framesPerSecond;
        }
        if (pattern[info.AttackPatternIndex] == 0 && now > info.FrameForNextInstruction) IncrementDefenderAttackPattern(info);
    }
};
int main()
{
    long entries[2] = { 1, 0 }; CCombatRing ring = { 0.0f, 2.5f };
    CAttackPatternDef def; def.AttackPattern.first = entries; def.AttackPattern.last = entries + 2;
    CCombatantInfo info; info.CombatWheel.first = &ring; info.CombatWheel.last = &ring + 1; info.PAttackPattern = &def;
    info.AttackPatternIndex = 0; info.FrameForNextInstruction = 50;
    CCombatManager m; m.AttackPatternIdleStandardInterval = 1.0f; m.increments = 0;
    m.UpdateDefenderAttackInstruction(info);             // attack entry: ring stopped, no step
    if (ring.DeltaRotation != 0.0f || m.increments != 0) { std::printf("006e86a0_TEST FAIL\n"); return 1; }
    info.AttackPatternIndex = 1; info.FrameForNextInstruction = 200;
    m.UpdateDefenderAttackInstruction(info);             // idle, wait not over
    if (m.increments != 0) { std::printf("006e86a0_TEST FAIL\n"); return 1; }
    info.FrameForNextInstruction = 99;
    m.UpdateDefenderAttackInstruction(info);             // idle, wait over: step (wraps to 0)
    if (m.increments != 1 || info.AttackPatternIndex != 0) { std::printf("006e86a0_TEST FAIL\n"); return 1; }
    std::printf("006e86a0_TEST PASS\n");
    return 0;
}