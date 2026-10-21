#include <cstdio>
#include <cstring>
// Behaviour model of CCham::Main (00ef3aa0).
struct CScriptThing { int id; };
static int g_frame, g_terminateAt, g_startOkAt, g_scared, g_moveTo, g_routes[16], g_routeCount, g_scripting, g_releases;
struct CHandle {
    bool IsScripting() { return g_scripting != 0; }
    void Release() { ++g_releases; g_scripting = 0; }
    void MoveToThing(const CScriptThing& t, float d, int type, bool a, bool b, bool c, bool e) { if (d == 0.0f && type == 0 && a && b && !c && e) g_moveTo = t.id; }
    void FollowPreCalculatedRoute(const CScriptThing& t, int type, bool a, bool b) { if (type == 1 && a && !b && g_routeCount < 16) g_routes[g_routeCount++] = t.id; }
};
struct CGameScriptInterface {
    void NewScriptFrame() const { ++g_frame; }
    bool StartScriptingEntity(const CScriptThing&, CHandle&, int priority) { if (priority == 4 && g_frame >= g_startOkAt) { g_scripting = 1; return true; } return false; }
    CScriptThing GetThingWithScriptName(const char* name) const { CScriptThing t; t.id = std::strcmp(name, "InMap1") == 0 ? 1 : std::strcmp(name, "InMap2") == 0 ? 2 : 0; return t; }
    void EntitySetAsScared(const CScriptThing&, bool scared) const { g_scared = scared; }
};
struct CCham {
    CGameScriptInterface* Game; CScriptThing Me;
    bool IsActiveThreadTerminating() const { return g_frame >= g_terminateAt; }
    void Main()
    {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        CScriptThing inMap1 = Game->GetThingWithScriptName("InMap1");
        CScriptThing inMap2 = Game->GetThingWithScriptName("InMap2");
        Game->EntitySetAsScared(Me, true);
        CHandle scripted;
        if (scripted.IsScripting()) scripted.Release();
        while (!Game->StartScriptingEntity(Me, scripted, 4)) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
        if (IsActiveThreadTerminating()) return;
        scripted.MoveToThing(inMap1, 0.0f, 0, true, true, false, true);
        while (!IsActiveThreadTerminating()) {
            scripted.FollowPreCalculatedRoute(inMap1, 1, true, false);
            scripted.FollowPreCalculatedRoute(inMap2, 1, true, false);
            Game->NewScriptFrame();
        }
    }
};
int main()
{
    CGameScriptInterface game; CCham c; c.Game = &game; c.Me.id = 9;
    g_terminateAt = 6; g_startOkAt = 2;
    c.Main();
    // frame 1 start, scripting from frame 2, follow loop runs frames 2..5 (4 iterations, 8 routes)
    if (!g_scared || g_moveTo != 1 || g_routeCount != 8 || g_routes[0] != 1 || g_routes[1] != 2 || g_routes[7] != 2 || g_frame != 6) {
        std::printf("00ef3aa0_TEST FAIL\n"); return 1;
    }
    std::printf("00ef3aa0_TEST PASS\n");
    return 0;
}
