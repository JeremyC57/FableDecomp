// Behaviour test for CThing::FrameUpdate @ 0x004C9D20: bullet-time update only when active, not a full frame and affected.
#include <cstdio>
struct CBulletTimeManager {
    long m_bulletTimeInfoCount; bool Override; bool fullFrame;
    bool IsActive() const { return Override || m_bulletTimeInfoCount; }
    bool IsTimeForFullUpdate() { return fullFrame; }
};
struct CWorldTime { CBulletTimeManager* m_pBulletTimeManager; };
struct CThingManager { CWorldTime* m_pWorldTime; };
static CBulletTimeManager g_bt;
static CWorldTime g_time;
static CThingManager g_manager;
static CThingManager* g_pThingManager = &g_manager;
struct CThing {
    int normal, bullet; bool affected;
    virtual void WorldUpdate() { ++normal; }
    virtual void WorldUpdateBulletTime() { ++bullet; }
    bool IsAffectedByBulletTime() { return affected; }
    void FrameUpdate()
    {
        CBulletTimeManager* bulletTime = g_pThingManager->m_pWorldTime->m_pBulletTimeManager;
        if (bulletTime->IsActive() && !bulletTime->IsTimeForFullUpdate() && IsAffectedByBulletTime())
            WorldUpdateBulletTime();
        else
            WorldUpdate();
    }
};
static bool Run(bool ovr, long count, bool full, bool affected, bool wantBullet)
{
    g_bt.Override = ovr; g_bt.m_bulletTimeInfoCount = count; g_bt.fullFrame = full;
    CThing t; t.normal = 0; t.bullet = 0; t.affected = affected;
    t.FrameUpdate();
    return wantBullet ? (t.bullet == 1 && t.normal == 0) : (t.bullet == 0 && t.normal == 1);
}
int main()
{
    g_manager.m_pWorldTime = &g_time; g_time.m_pBulletTimeManager = &g_bt;
    if (!Run(false, 0, false, true, false) || !Run(true, 0, false, true, true) || !Run(false, 2, false, true, true) ||
        !Run(true, 0, true, true, false) || !Run(true, 0, false, false, false)) {
        std::printf("004c9d20_TEST FAIL\n"); return 1;
    }
    std::printf("004c9d20_TEST PASS\n");
    return 0;
}
