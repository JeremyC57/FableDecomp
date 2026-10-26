// Behaviour test for CAnimationEntry::GetTimeToBlendIn @ 0x005dc230: component value, scaled (truncated) by bullet time when affected and active.
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
class CAnimComponentTransitionInTime { public: unsigned char _pad[8]; long TransitionInTime; };
static CAnimComponentTransitionInTime g_component;
class CAnimationEntry { public:
    long BankIndex; long AnimName; long GroupName; void* Components;
    void GetTransitionInTimeComponent(CAnimComponentTransitionInTime** ppComponent) const { *ppComponent = &g_component; }
    long GetAnimationTransitionInTime() const { CAnimComponentTransitionInTime* p; GetTransitionInTimeComponent(&p); return p->TransitionInTime; }
    long GetTimeToBlendIn(CThing* thing) const
    {
        long time = GetAnimationTransitionInTime();
        if (thing->IsAffectedByBulletTime()) { CBulletTime* bt = g_pApplication->m_pGame->m_pBulletTime; if (bt->IsActive()) return (long)(bt->GetTimeScale() * time); }
        return time;
    }
};
int main()
{
    CBulletTime bt; bt.m_pController = 0; bt.m_active = false; bt.m_scale = 0.25f;
    CGame game; game.m_pBulletTime = &bt; CApplication app; app.m_pGame = &game; g_pApplication = &app;
    g_component.TransitionInTime = 250;
    CAnimationEntry e; CThing thing; thing.m_affected = false;
    bool ok = e.GetTimeToBlendIn(&thing) == 250;
    thing.m_affected = true; if (e.GetTimeToBlendIn(&thing) != 250) ok = false;      // bullet time inactive
    bt.m_active = true; if (e.GetTimeToBlendIn(&thing) != 62) ok = false;             // 250 * 0.25 = 62.5 -> 62
    bt.m_active = false; bt.m_pController = &bt; if (e.GetTimeToBlendIn(&thing) != 62) ok = false;
    if (!ok) { std::printf("005dc230_TEST FAIL\n"); return 1; }
    std::printf("005dc230_TEST PASS\n");
    return 0;
}