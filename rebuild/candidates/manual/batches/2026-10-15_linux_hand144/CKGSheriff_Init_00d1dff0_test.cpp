// Behaviour test for 0x00d1dff0 (CKG_Sheriff::Init): the reconstructed logic run over a host model of its callees.
#include <cstdio>
#include <cstring>
#include <cmath>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)
struct CScriptThing { const char* name; };
struct CGameScriptInterface {
    const char* attached; const char* village; const char* hidden; bool hiddenFlag;
    CScriptThing GetThingWithScriptName(const char* n) const { CScriptThing t; t.name = n; return t; }
    void EntityAttachToVillage(const CScriptThing& thing, const CScriptThing& v) { attached = thing.name; village = v.name; }
    void SetDiggingSpotAsHidden(const CScriptThing& spot, bool h) { hidden = spot.name; hiddenFlag = h; }
};
struct CKG_Sheriff {
    CGameScriptInterface* Interface; CScriptThing Me; bool ToldHeroToGatherGuards;
    void Init() { ToldHeroToGatherGuards = false; Interface->EntityAttachToVillage(Me, Interface->GetThingWithScriptName("VILLAGE_KNOTHOLEGLADE")); }
};
static void Run()
{
    CGameScriptInterface gsi; gsi.attached = 0; gsi.village = 0;
    CKG_Sheriff s; s.Interface = &gsi; s.Me.name = "Sheriff"; s.ToldHeroToGatherGuards = true;
    s.Init();
    CHECK(!s.ToldHeroToGatherGuards && std::strcmp(gsi.attached, "Sheriff") == 0 && std::strcmp(gsi.village, "VILLAGE_KNOTHOLEGLADE") == 0);
}

int main()
{
    Run();
    if (g_fail) { std::printf("00d1dff0_TEST FAIL\n"); return 1; }
    std::printf("00d1dff0_TEST PASS\n");
    return 0;
}
