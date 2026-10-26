// Behaviour test for 0x00783C50 CTCBoastingCrowdControl::OnHeroBoast: an existing crowd receives the boast.
#include <cstdio>
class CThing { public: long m_id; };
static int g_events[8];
static int g_ne;
static long g_boast;
class CTCCreatureGroupBoastingCrowd { public:
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
class CTCBoastingCrowdControl { public:
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
    void OnHeroBoast(long boast)
    {
        CThing* parent = ParentThing;
        CThing* groupThing;
        CTCCreatureGroupBoastingCrowd* crowd;
        if (GetCrowd(parent, &groupThing, &crowd, false))
            crowd->OnHeroBoast(boast);
    }
};

int main()
{
    CThing parent;
    CTCBoastingCrowdControl c; c.ParentThing = &parent;
    g_haveCrowd = false; g_ne = 0; g_boast = 0;
    c.OnHeroBoast(5);
    if (g_askedThing != &parent || g_askedCreate || g_ne != 0 || g_boast != 0) { std::printf("00783c50_TEST FAIL\n"); return 1; }
    g_haveCrowd = true;
    c.OnHeroBoast(9);
    if (g_ne != 1 || g_events[0] != 4 || g_boast != 9) { std::printf("00783c50_TEST FAIL\n"); return 1; }
    std::printf("00783c50_TEST PASS\n");
    return 0;
}