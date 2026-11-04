#include <cstdio>
#include <cstring>
// Behaviour test for CM_RestartMarker::Main (0x00EF34A0): scripted control retried each frame, then per frame: speed 1.5,
// walk to Marker1, reset speed, walk to Marker2, until the thread terminates.
static char g_log[512];
static void Log(const char* s) { std::strcat(g_log, s); }
class CScriptThing { public: const char* Name; };
struct CScripted {
    bool Scripting;
    CScripted() : Scripting(false) { }
    bool IsScripting() { return Scripting; }
    void Release() { Log("R"); }
    void MoveToThing(const CScriptThing& t, float, int, bool, bool, bool, bool) { Log(t.Name); }
};
class CGame { public:
    int Frames, StartAfter, TerminateAt;
    void NewScriptFrame() { Frames++; Log("F"); }
    bool StartScriptingEntity(const CScriptThing&, CScripted&, int priority) { return priority == 4 && Frames >= StartAfter; }
    CScriptThing GetThingWithScriptName(const char* n) { CScriptThing t = { n[6] == '1' ? "1" : "2" }; return t; }
    void EntitySetMaxWalkingSpeed(const CScriptThing&, float s) { if (s == 1.5f) Log("S"); }
    void EntityResetMaxWalkingSpeed(const CScriptThing&) { Log("s"); }
};
struct CM_RestartMarker {
    CGame* Game; CScriptThing Me;
    bool IsActiveThreadTerminating() const { return Game->Frames >= Game->TerminateAt; }
    void Main()
    {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        CScripted scripted;
        if (scripted.IsScripting()) scripted.Release();
        while (!Game->StartScriptingEntity(Me, scripted, 4)) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
        if (IsActiveThreadTerminating()) return;
        CScriptThing marker1 = Game->GetThingWithScriptName("Marker1");
        CScriptThing marker2 = Game->GetThingWithScriptName("Marker2");
        while (!IsActiveThreadTerminating()) {
            Game->EntitySetMaxWalkingSpeed(Me, 1.5f);
            scripted.MoveToThing(marker1, 0.0f, 0, true, true, false, true);
            Game->EntityResetMaxWalkingSpeed(Me);
            scripted.MoveToThing(marker2, 0.0f, 0, true, true, false, true);
            Game->NewScriptFrame();
        }
    }
};
int main()
{
    CGame g = { 0, 2, 4 };
    CScriptThing me = { "me" };
    CM_RestartMarker m = { &g, me };
    m.Main();
    if (std::strcmp(g_log, "FFS1s2FS1s2F") != 0) { std::printf("00ef34a0_TEST FAIL\n"); return 1; }
    g_log[0] = 0;
    CGame h = { 0, 9, 3 };
    m.Game = &h;
    m.Main();
    if (std::strcmp(g_log, "FFF") != 0) { std::printf("00ef34a0_TEST FAIL\n"); return 1; }
    std::printf("00ef34a0_TEST PASS\n");
    return 0;
}