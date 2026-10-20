// Behaviour test for CV_DemonDoorsScript::FadeOutFaceAndOpenDoor (00e6f040): call order and arguments.
#include <cstdio>
#include <cstring>
struct CCharString { const char* t; };
struct CScriptThing { int id; };
static char g_log[128];
static void Log(const char* s) { std::strcat(g_log, s); }
static const CScriptThing* g_faded; static float g_secs; static bool g_kill, g_fadeIn;
void FadeOutAndKillEntity(const CScriptThing& t, float s, bool k, bool f) { g_faded = &t; g_secs = s; g_kill = k; g_fadeIn = f; Log("F"); }
struct FakeGSI {
    int opened, persisted; const CScriptThing* removed; bool ra, rb;
    CScriptThing GetThingWithScriptName(const CCharString& n) const { Log("G"); CScriptThing t; t.id = std::strcmp(n.t, "Door") == 0 ? 7 : -1; return t; }
    void OpenDoor(const CScriptThing& d) { opened = d.id; Log("O"); }
    void SetThingPersistent(const CScriptThing& d, bool p) { persisted = p ? d.id : 0; Log("P"); }
    void RemoveThing(const CScriptThing& t, bool a, bool b) { removed = &t; ra = a; rb = b; Log("R"); }
};
struct Script {
    FakeGSI* GSI;
    void FadeOutFaceAndOpenDoor(const CScriptThing& face, const CCharString& doorName)
    {
        FadeOutAndKillEntity(face, 3.0f, false, true);
        CScriptThing door = GSI->GetThingWithScriptName(doorName);
        GSI->OpenDoor(door);
        GSI->SetThingPersistent(door, true);
        GSI->RemoveThing(face, false, true);
    }
};
int main()
{
    FakeGSI g = { 0, 0, 0, true, false };
    Script s = { &g };
    CScriptThing face = { 3 }; CCharString name = { "Door" };
    s.FadeOutFaceAndOpenDoor(face, name);
    bool ok = !std::strcmp(g_log, "FGOPR") && g_faded == &face && g_secs == 3.0f && !g_kill && g_fadeIn && g.opened == 7
        && g.persisted == 7 && g.removed == &face && !g.ra && g.rb;
    if (!ok) { std::printf("00e6f040_TEST FAIL\n"); return 1; }
    std::printf("00e6f040_TEST PASS\n");
    return 0;
}
