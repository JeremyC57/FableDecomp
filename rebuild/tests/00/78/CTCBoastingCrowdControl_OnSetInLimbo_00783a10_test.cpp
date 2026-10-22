// Behaviour test for 0x00783A10 CTCBoastingCrowdControl::vfunc_16: the flag kills every crowd thing; without
// it an inactive control starts up and an active one does nothing.
#include <cstdio>
struct CThing { long m_id; };
static int g_events[8];
static int g_ne;
static long g_boast;
struct CTCCreatureGroupBoastingCrowd {
    void OnHeroApproachBoastingArea() { g_events[g_ne++] = 1; }
    void OnHeroEnterBoastingArea() { g_events[g_ne++] = 2; }
    void OnHeroLeaveBoastingArea() { g_events[g_ne++] = 3; }
    void OnHeroBoast(long boast) { g_events[g_ne++] = 4; g_boast = boast; }
    void OnHeroAttackCrowdMember() { g_events[g_ne++] = 5; }
};
static CTCCreatureGroupBoastingCrowd g_crowd;
static CThing g_groupThing;
static bool g_haveCrowd;
static CThing* g_askedThing;
static bool g_askedCreate;
struct CTCBoastingCrowdControl {
    CThing* ParentThing;
    bool    Active;
    bool    CrowdSizesSuppressed;
    bool GetCrowd(CThing* thing, CThing** groupThing, CTCCreatureGroupBoastingCrowd** crowd, bool create)
    {
        g_askedThing = thing; g_askedCreate = create;
        if (!g_haveCrowd && !create) return false;
        *groupThing = &g_groupThing; *crowd = &g_crowd;
        return true;
    }
    int m_kills, m_startups;
    void KillAllCrowdThings() { ++m_kills; }
    void Startup() { ++m_startups; }
    void OnSetInLimbo(bool flag)
    {
        if (flag)
            KillAllCrowdThings();
        else if (!Active)
            Startup();
    }
};

int main()
{
    CTCBoastingCrowdControl c; c.m_kills = 0; c.m_startups = 0;
    c.Active = false;
    c.OnSetInLimbo(true);
    if (c.m_kills != 1 || c.m_startups != 0) { std::printf("00783a10_TEST FAIL\n"); return 1; }
    c.OnSetInLimbo(false);
    if (c.m_kills != 1 || c.m_startups != 1) { std::printf("00783a10_TEST FAIL\n"); return 1; }
    c.Active = true;
    c.OnSetInLimbo(false);
    if (c.m_kills != 1 || c.m_startups != 1) { std::printf("00783a10_TEST FAIL\n"); return 1; }
    std::printf("00783a10_TEST PASS\n");
    return 0;
}