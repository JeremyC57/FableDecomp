#include <cstdio>
#include <cstring>

struct CGameSubsystem { bool m_suppressed; bool IsSuppressed() { return m_suppressed; } };
class CWorld { public: unsigned long m_seed; unsigned long* GetPRandomSeed() { return &m_seed; } };
static float g_nextRandom = 0.0f; static float g_lastRange = 0.0f; static unsigned long* g_lastSeed = 0;
static float GFFloatRandom(float range, unsigned long* seed) { g_lastRange = range; g_lastSeed = seed; return g_nextRandom; }
class CThing { public: int kills; bool lastImmediate; void Kill(bool immediately) { ++kills; lastImmediate = immediately; } };
static CGameSubsystem g_subsystem; static CWorld g_world;
class CTCBase { public: CThing* ParentThing; CWorld* GetWorld() { return &g_world; } CGameSubsystem* GetSubsystem() { return &g_subsystem; } };
struct CTCAmbientBird : CTCBase {
    void OnInitialActivate()
    {
        if (!GetSubsystem()->IsSuppressed()) {
            if (GFFloatRandom(1.0f, GetWorld()->GetPRandomSeed()) > 0.5f)
                ParentThing->Kill(false);
        }
    }
};
int main()
{
    CThing bird; bird.kills = 0; bird.lastImmediate = true;
    CTCAmbientBird b; b.ParentThing = &bird;
    g_subsystem.m_suppressed = false; g_nextRandom = 0.75f; b.OnInitialActivate();
    if (bird.kills != 1 || bird.lastImmediate || g_lastRange != 1.0f || g_lastSeed != &g_world.m_seed) { std::printf("007e1f70_TEST FAIL\n"); return 1; }
    g_nextRandom = 0.5f; b.OnInitialActivate();
    if (bird.kills != 1) { std::printf("007e1f70_TEST FAIL\n"); return 1; }
    g_subsystem.m_suppressed = true; g_nextRandom = 0.9f; b.OnInitialActivate();
    if (bird.kills != 1) { std::printf("007e1f70_TEST FAIL\n"); return 1; }
    std::printf("007e1f70_TEST PASS\n");
    return 0;
}