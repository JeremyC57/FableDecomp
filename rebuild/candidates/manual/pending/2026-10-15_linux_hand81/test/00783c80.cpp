// Behaviour test for 0x00783C80 CTCBoastingCrowdControl::OnHeroAttackCrowdMember: an existing crowd is told,
// and the crowd sizes are suppressed in either case.
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
    void OnHeroAttackCrowdMember()
    {
        CThing* parent = ParentThing;
        CThing* groupThing;
        CTCCreatureGroupBoastingCrowd* crowd;
        if (GetCrowd(parent, &groupThing, &crowd, false))
            crowd->OnHeroAttackCrowdMember();
        CrowdSizesSuppressed = true;
    }
};

int main()
{
    CThing parent;
    CTCBoastingCrowdControl c; c.ParentThing = &parent; c.CrowdSizesSuppressed = false;
    g_haveCrowd = false; g_ne = 0;
    c.OnHeroAttackCrowdMember();
    if (g_askedCreate || g_ne != 0 || !c.CrowdSizesSuppressed) { std::printf("00783c80_TEST FAIL\n"); return 1; }
    c.CrowdSizesSuppressed = false; g_haveCrowd = true;
    c.OnHeroAttackCrowdMember();
    if (g_ne != 1 || g_events[0] != 5 || !c.CrowdSizesSuppressed) { std::printf("00783c80_TEST FAIL\n"); return 1; }
    std::printf("00783c80_TEST PASS\n");
    return 0;
}
