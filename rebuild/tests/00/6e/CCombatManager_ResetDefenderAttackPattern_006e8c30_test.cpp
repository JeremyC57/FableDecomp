#include <cstdio>
// Behaviour test for CCombatManager::ResetDefenderAttackPattern @ 0x006E8C30.
static long g_framesPerSecond = 15;
static int g_destroyed = 0;
struct Def { long m_count; void Release() { if (--m_count == 0) ++g_destroyed; } };
struct DefPtr {
    Def* m_p;
    DefPtr(Def* p) : m_p(p) { if (m_p) ++m_p->m_count; }
    DefPtr(const DefPtr& o) : m_p(o.m_p) { if (m_p) ++m_p->m_count; }
    ~DefPtr() { if (m_p) m_p->Release(); }
    DefPtr& operator=(const DefPtr& o)
    {
        if (m_p != o.m_p) { if (m_p) { m_p->Release(); m_p = 0; } m_p = o.m_p; if (m_p) ++m_p->m_count; }
        return *this;
    }
};
struct Ring { float RotationAngle, DeltaRotation; };
struct Info {
    Ring* first; Ring* last; DefPtr PAttackPattern; long AttackPatternIndex, FrameForNextInstruction;
    Info() : first(0), last(0), PAttackPattern(0), AttackPatternIndex(0), FrameForNextInstruction(0) { }
    Ring* GetRing(long index) { if (index >= 0 && (unsigned long)index < (unsigned long)(last - first)) return &first[index]; return 0; }
};
static void ResetDefenderAttackPattern(Info& info, DefPtr pattern)
{
    info.PAttackPattern = pattern;
    info.AttackPatternIndex = 0;
    info.FrameForNextInstruction = 0;
    long i = 0; Ring* ring;
    while ((ring = info.GetRing(i++)) != 0) ring->DeltaRotation = 0.0f / g_framesPerSecond;
}
static int Fail() { std::printf("006e8c30_TEST FAIL\n"); return 1; }
int main()
{
    Ring rings[3] = {{1, 2}, {3, 4}, {5, 6}};
    Def oldDef = {0}, newDef = {0};
    Info info; info.first = rings; info.last = rings + 3;
    info.PAttackPattern = DefPtr(&oldDef);
    info.AttackPatternIndex = 4; info.FrameForNextInstruction = 99;
    { DefPtr p(&newDef); ResetDefenderAttackPattern(info, p); }
    if (info.PAttackPattern.m_p != &newDef || newDef.m_count != 1 || oldDef.m_count != 0 || g_destroyed != 1) return Fail();
    if (info.AttackPatternIndex != 0 || info.FrameForNextInstruction != 0) return Fail();
    for (int k = 0; k < 3; ++k) if (rings[k].DeltaRotation != 0.0f || rings[k].RotationAngle != 1 + 2 * k) return Fail();
    std::printf("006e8c30_TEST PASS\n");
    return 0;
}