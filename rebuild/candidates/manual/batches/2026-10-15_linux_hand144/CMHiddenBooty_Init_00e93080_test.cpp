// Behaviour test for 0x00e93080 (CM_HiddenBooty::Init): the reconstructed logic run over a host model of its callees.
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
struct CM_HiddenBooty {
    CGameScriptInterface* Interface; bool f1c, f1d, f1e;
    void Init() { f1c = false; f1d = false; f1e = false; Interface->SetDiggingSpotAsHidden(Interface->GetThingWithScriptName("M_HiddenBooty"), true); }
};
static void Run()
{
    CGameScriptInterface gsi; gsi.hidden = 0; gsi.hiddenFlag = false;
    CM_HiddenBooty b; b.Interface = &gsi; b.f1c = b.f1d = b.f1e = true;
    b.Init();
    CHECK(!b.f1c && !b.f1d && !b.f1e && gsi.hiddenFlag && std::strcmp(gsi.hidden, "M_HiddenBooty") == 0);
}

int main()
{
    Run();
    if (g_fail) { std::printf("00e93080_TEST FAIL\n"); return 1; }
    std::printf("00e93080_TEST PASS\n");
    return 0;
}
