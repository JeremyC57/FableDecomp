// Behaviour test for 0x00782770 CTCBoastingArea::CTCBoastingArea: state outside, radius 3, anticipation 10.
#include <cstdio>
#include <cstring>
#include <new>
struct CThing { long m_id; };

static int g_baseCtor;
struct CTCBase_Calls {
    virtual void v00() { }
    CThing* ParentThing;
    bool    ToBeRemoved;
    bool    IsInGlobalUpdateTCs;
    CTCBase_Calls(CThing* thing) : ParentThing(thing), ToBeRemoved(false), IsInGlobalUpdateTCs(false) { ++g_baseCtor; }
};

static double g_storage[64];
static void* Fresh() { std::memset(g_storage, 0xAB, sizeof g_storage); return g_storage; }
enum EHeroWithinRadiusState { HERO_OUTSIDE_RADIUS = 0 };
struct CTCBoastingArea : CTCBase_Calls {
    EHeroWithinRadiusState HeroWithinRadiusState;
    float                  Radius;
    float                  AnticipationRadius;
    CTCBoastingArea(CThing* thing) : CTCBase_Calls(thing), HeroWithinRadiusState(HERO_OUTSIDE_RADIUS), Radius(3.0f), AnticipationRadius(10.0f) { }
};

int main()
{
    CThing thing;
    CTCBoastingArea* p = new (Fresh()) CTCBoastingArea(&thing);
    if (g_baseCtor != 1 || p->ParentThing != &thing || p->HeroWithinRadiusState != HERO_OUTSIDE_RADIUS || p->Radius != 3.0f || p->AnticipationRadius != 10.0f) { std::printf("00782770_TEST FAIL\n"); return 1; }
    std::printf("00782770_TEST PASS\n");
    return 0;
}
