// Behaviour test for CAnimEntryLite::GetTimeToBlendIn @ 0x0073a690: cached TransitionTime, scaled (truncated) by bullet time when affected and active.
#include <cstdio>
class CThing { public: bool m_affected; bool IsAffectedByBulletTime() { return m_affected; } };
struct CBulletTime {
    unsigned char _pad0[4]; void* m_pController; unsigned char _pad1[4]; bool m_active; float m_scale;
    float GetTimeScale() { return m_scale; }
    bool IsActive() const { return m_active || m_pController != 0; }
};
class CGame { public: unsigned char _pad[0x68]; CBulletTime* m_pBulletTime; };
struct CApplication { unsigned char _pad[0x24]; CGame* m_pGame; };
static CApplication* g_pApplication;
struct CAnimEntryLite {
    long BankIndex; long DelayTime; long TransitionTime; long AnimFlags;
    long GetTimeToBlendIn(CThing* thing) const
    {
        long time = TransitionTime;
        if (thing->IsAffectedByBulletTime()) { CBulletTime* bt = g_pApplication->m_pGame->m_pBulletTime; if (bt->IsActive()) return (long)(bt->GetTimeScale() * time); }
        return time;
    }
};
int main()
{
    CBulletTime bt; bt.m_pController = 0; bt.m_active = false; bt.m_scale = 2.5f;
    CGame game; game.m_pBulletTime = &bt; CApplication app; app.m_pGame = &game; g_pApplication = &app;
    CAnimEntryLite e; e.TransitionTime = 101; CThing thing; thing.m_affected = false;
    bool ok = e.GetTimeToBlendIn(&thing) == 101;
    thing.m_affected = true; if (e.GetTimeToBlendIn(&thing) != 101) ok = false;
    bt.m_active = true; if (e.GetTimeToBlendIn(&thing) != 252) ok = false;            // 101 * 2.5 = 252.5 -> 252
    if (!ok) { std::printf("0073a690_TEST FAIL\n"); return 1; }
    std::printf("0073a690_TEST PASS\n");
    return 0;
}