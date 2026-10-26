// Behaviour test for CAIStateGroup_JackDragon::StayIdle (0x0094FE30): idles one frame per DoNothing until
// `seconds` have elapsed at g_framesPerSecond; a dead enemy or a stopping group terminates immediately.
#include <cstdio>

long g_framesPerSecond = 30;
static long g_frame = 100;
static int g_idles = 0, g_terminates = 0, g_dieAfter = -1;
static bool g_stopping = false;

class CThing { public: unsigned char m_flags91; };
struct CBaseIntelligentPointer {
    CThing* m_pItem;
    CThing* GetPItem() const { return m_pItem; }
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
class CWorld { public: static long GetWorldFrame() { return g_frame; } };
static CWorld g_world;
static CThing g_enemy;

struct CAIStateGroup_JackDragon {
    CBaseIntelligentPointer PEnemy;
    CWorld* GetPWorld() { return &g_world; }
    long AIAction_DoNothing() { ++g_frame; ++g_idles; if (g_idles == g_dieAfter) g_enemy.m_flags91 = 1; return 0; }
    bool IsStopping() { return g_stopping; }
    void Terminate() { ++g_terminates; }
    void StayIdle(float seconds)
    {
        long start = GetPWorld()->GetWorldFrame();
        do {
            AIAction_DoNothing();
            if (!IS_THING_ALIVE(PEnemy) || IsStopping()) {
                Terminate();
                return;
            }
        } while ((float)(GetPWorld()->GetWorldFrame() - start) / g_framesPerSecond < seconds);
    }
};

int main()
{
    g_enemy.m_flags91 = 0;
    CAIStateGroup_JackDragon g; g.PEnemy.m_pItem = &g_enemy;
    g.StayIdle(1.0f);                                   // 30 frames at 30 fps
    bool ok = g_idles == 30 && g_terminates == 0;
    g_idles = 0; g_dieAfter = 5; g.StayIdle(2.0f);      // enemy dies on the 5th frame
    ok = ok && g_idles == 5 && g_terminates == 1;
    g_idles = 0; g_dieAfter = -1; g_enemy.m_flags91 = 0; g_stopping = true; g.StayIdle(2.0f);
    ok = ok && g_idles == 1 && g_terminates == 2;
    g_stopping = false; g_idles = 0; g.PEnemy.m_pItem = 0; g.StayIdle(2.0f);
    ok = ok && g_idles == 1 && g_terminates == 3;
    if (!ok) { std::printf("0094fe30_TEST FAIL\n"); return 1; }
    std::printf("0094fe30_TEST PASS\n");
    return 0;
}