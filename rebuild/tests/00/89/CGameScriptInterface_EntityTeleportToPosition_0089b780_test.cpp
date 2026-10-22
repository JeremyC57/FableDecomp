// Behaviour test for 0x0089B780: CGameScriptInterface::EntityTeleportToPosition branch logic (hero cross-map event, walker move, generator move, carrying update).
#include <cstdio>
#include <cstring>
static char g_log[2048];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct Vec { float X, Y, Z; };
struct Obj {
    int Type; int Flags6c; int Flags24; int Flags28; bool hasWalk; bool creature; int player;
};
struct World {
    int regionFrom, regionTo; int mapFrom, mapTo; bool allowLoad; bool cameraOk;
    int evtType; float evtF[4]; bool evtB[3]; int evtPlayer;
};
static World W;
static bool g_scripted;
static bool g_groundHit;
static void Teleport(bool valid, Obj* o, const Vec& pos, float angle, bool fade, bool keepOnGround)
{
    if (!valid) return;
    int fromMap = W.mapFrom;
    int toMap = W.mapTo;
    if (toMap <= 0) return;
    if (o->Type == 2 && fromMap != toMap) {
        W.allowLoad = false;
        W.evtType = 0x2c;
        W.evtF[0] = pos.X; W.evtF[1] = pos.Y; W.evtF[2] = pos.Z; W.evtF[3] = angle;
        W.evtB[0] = fade; W.evtB[1] = true; W.evtB[2] = false;
        W.evtPlayer = o->player;
        Log("event");
    } else if (o->Flags6c & 8) {
        bool walk = false;
        if (o->hasWalk) { walk = true; Log("reset"); }
        Log("unpersist");
        bool creature = o->creature;
        if (creature) Log("limbo+start");
        Log("setpos");
        if (keepOnGround && walk) { if (g_groundHit) Log("ground"); }
        if (creature) Log("finish+unlimbo");
        if (o->Flags24 & 0x100000) Log("perceive");
        if (creature && !g_scripted) { if (W.cameraOk) Log("camreset"); }
        Log("heading");
    } else {
        if (o->Flags24 & 0x20000000) Log("generator");
        Log("setpos");
        Log("heading");
    }
    if (o->Flags28 & 0x40) Log("carry");
}
static int Check(const char* want) { int ok = std::strcmp(g_log, want) == 0; if (!ok) std::printf("got %s want %s\n", g_log, want); g_log[0] = 0; return ok; }
int main()
{
    int ok = 1;
    Vec p; p.X = 1.0f; p.Y = 2.0f; p.Z = 3.0f;
    Obj hero; std::memset(&hero, 0, sizeof hero); hero.Type = 2; hero.player = 1;
    W.mapFrom = 1; W.mapTo = 2; W.allowLoad = true; W.cameraOk = true;
    Teleport(false, &hero, p, 0.5f, true, false); ok &= Check("");
    Teleport(true, &hero, p, 0.5f, true, false); ok &= Check("event;");
    ok &= W.evtType == 0x2c && W.evtF[3] == 0.5f && W.evtB[0] && W.evtB[1] && !W.evtB[2] && W.evtPlayer == 1 && !W.allowLoad;
    W.mapTo = 0; Teleport(true, &hero, p, 0.5f, true, false); ok &= Check("");
    W.mapTo = 1;
    Obj w; std::memset(&w, 0, sizeof w); w.Flags6c = 8; w.hasWalk = true; w.creature = true; w.Flags24 = 0x100000; w.Flags28 = 0x40;
    g_groundHit = true; g_scripted = false;
    Teleport(true, &w, p, 1.0f, false, true);
    ok &= Check("reset;unpersist;limbo+start;setpos;ground;finish+unlimbo;perceive;camreset;heading;carry;");
    g_scripted = true; w.hasWalk = false;
    Teleport(true, &w, p, 1.0f, false, true);
    ok &= Check("unpersist;limbo+start;setpos;finish+unlimbo;perceive;heading;carry;");
    Obj g; std::memset(&g, 0, sizeof g); g.Flags24 = 0x20000000;
    Teleport(true, &g, p, 1.0f, false, false); ok &= Check("generator;setpos;heading;");
    hero.Type = 2; Teleport(true, &hero, p, 1.0f, false, false); ok &= Check("setpos;heading;");
    std::printf(ok ? "0089b780_TEST PASS\n" : "0089b780_TEST FAIL\n");
    return ok ? 0 : 1;
}