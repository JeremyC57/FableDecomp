#include <cstdio>
#ifndef _MSC_VER
#define __fastcall
#endif

// Model: one frame's update time, slowed by the bullet-time scale for affected things.
long g_framesPerSecond = 25;

class CThing {
public:
    bool Affected;
    bool IsAffectedByBulletTime() const { return Affected; }
};

struct CBulletTime {
    void* m_pController;
    bool  m_active;
    float m_scale;
    float GetTimeScale() { return m_scale; }
    bool IsActive() const { return m_active || m_pController != 0; }
};

struct CWorld {
    CBulletTime* m_pBulletTime;
};

static float __fastcall GetUpdateTimeForThing(const CThing& thing, const CWorld& world)
{
    float seconds = 1.0f / g_framesPerSecond;
    if (!world.m_pBulletTime->IsActive())
        return seconds;
    if (thing.IsAffectedByBulletTime())
        return seconds / world.m_pBulletTime->GetTimeScale();
    return seconds;
}

int main()
{
    CBulletTime bt;
    bt.m_pController = 0;
    bt.m_active = false;
    bt.m_scale = 0.25f;
    CWorld world;
    world.m_pBulletTime = &bt;
    CThing affected, immune;
    affected.Affected = true;
    immune.Affected = false;
    float base = 1.0f / 25;
    bool ok = GetUpdateTimeForThing(affected, world) == base && GetUpdateTimeForThing(immune, world) == base;
    bt.m_active = true;
    ok = ok && GetUpdateTimeForThing(affected, world) == base / 0.25f && GetUpdateTimeForThing(immune, world) == base;
    bt.m_active = false;
    bt.m_pController = &bt;                              // a controller alone also counts as active
    ok = ok && GetUpdateTimeForThing(affected, world) == base / 0.25f;
    if (!ok) {
        std::printf("00787c62_TEST FAIL\n");
        return 1;
    }
    std::printf("00787c62_TEST PASS\n");
    return 0;
}