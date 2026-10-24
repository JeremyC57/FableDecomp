// Behaviour test for CTCCreatureOpinionOfHero::IsWillingToStartFollowingHero (0x006818A0): the
// creature starts following for attitudes 1, 2, 5, 6, 9, 11..15 and refuses for 0, 3, 4, 7, 8, 10,
// negative and >15 values (the retail byte index table).
#include <stdio.h>

struct CThing { int id; };

static long g_attitude = 0;
static const CThing* g_askedFor = 0;

struct COpinionReactionManager {
    long GetThingAttitudeToPlayer(const CThing* thing) const { g_askedFor = thing; return g_attitude; }
};

struct CWorld {
    unsigned char _pad00[0x74];
    COpinionReactionManager* POpinionReactionManager;
};

static COpinionReactionManager g_manager;
static CWorld g_world;

struct CTCCreatureOpinionOfHero {
    void* __vftable;
    CThing* ParentThing;
    CWorld* GetWorld() const { return &g_world; }
    bool IsWillingToStartFollowingHero() const;
};

bool CTCCreatureOpinionOfHero::IsWillingToStartFollowingHero() const
{
    CThing* thing = ParentThing;
    COpinionReactionManager* manager = GetWorld()->POpinionReactionManager;
    switch (manager->GetThingAttitudeToPlayer(thing)) {
    case 1:
    case 2:
    case 5:
    case 6:
    case 9:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
        return true;
    default:
        return false;
    }
}

int main()
{
    static const unsigned char expect[17] = { 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 1, 1, 1, 0 };
    CThing hero = { 7 };
    CTCCreatureOpinionOfHero tc;
    long a;
    g_world.POpinionReactionManager = &g_manager;
    tc.__vftable = 0;
    tc.ParentThing = &hero;
    for (a = -2; a <= 16; ++a) {
        bool want = (a >= 0 && a <= 16) ? (expect[a] != 0) : false;
        g_attitude = a;
        g_askedFor = 0;
        if (tc.IsWillingToStartFollowingHero() != want) { printf("FAIL attitude %ld\n", a); return 1; }
        if (g_askedFor != &hero) { printf("FAIL thing\n"); return 1; }
    }
    printf("CTCCreatureOpinionOfHero_006818a0_TEST PASS\n");
    return 0;
}
