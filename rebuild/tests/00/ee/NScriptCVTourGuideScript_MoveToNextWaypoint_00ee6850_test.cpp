#include <cstdio>
#include <cstring>
#include <string>
static std::string g_log;
static int g_frame = 0;
static int g_termAt = 1000;
static void (*g_hook)(int frame) = 0;
static void Log(const std::string& s) { char b[16]; std::sprintf(b, "@%d", g_frame); g_log += s; g_log += b; g_log += ";"; }
static void Reset(int termAt) { g_log = ""; g_frame = 0; g_termAt = termAt; g_hook = 0; }
class CCharString { public:
    std::string S;
    CCharString(const char* text, long length = -1) : S(text) { (void)length; }
};
class CScriptThing { public:
    std::string Name;
    CScriptThing() { }
    CScriptThing(const std::string& name) : Name(name) { }
};
class CScriptBase { public:
    bool IsActiveThreadTerminating() { return g_frame >= g_termAt; }
};
static int Fail(const char* addr, const char* what) { std::printf("%s_TEST FAIL %s [%s]\n", addr, what, g_log.c_str()); return 1; }
#define CHECK(addr, cond) do { if (!(cond)) return Fail(addr, #cond); } while (0)
// behaviour test: CV_TourGuideScript::MoveToNextWaypoint walks the guide to the given waypoint when it exists; otherwise
// it steps the waypoint counter (wrapping 18 -> 0) once a frame until it finds a live marker and walks there
class C3DVector { public: float X, Y, Z; };
enum EScriptEntityMoveType { MOVE_TYPE_WALK, MOVE_TYPE_RUN };
static std::string g_alive[4];
struct CScriptThingW : CScriptThing {
    float X;
    CScriptThingW() : X(0.0f) { }
    CScriptThingW(const std::string& n, float x) : CScriptThing(n), X(x) { }
    bool IsAlive() const { for (int i = 0; i < 4; ++i) if (g_alive[i] == Name) return true; return false; }
    C3DVector GetPos() const { C3DVector v; v.X = X; v.Y = v.Z = 0.0f; return v; }
};
class CGameScriptInterface { public:
    void NewScriptFrame() { ++g_frame; }
    CScriptThingW GetThingWithScriptName(const CCharString& name) { return CScriptThingW(name.S, (float)name.S.size()); }
};
class CScriptGameResourceObjectScriptedThingBase { public:
    void MoveToPosition(const C3DVector& p, float speed, EScriptEntityMoveType type, bool a, bool b)
    { char t[64]; std::sprintf(t, "move %g %g %d", p.X, speed, (int)type); Log(t); (void)a; (void)b; }
};
struct WaypointData { CCharString locMarker; long a, b; WaypointData() : locMarker("") { } };
struct CV_TourGuideScript : CScriptBase {
    CGameScriptInterface* Game;
    WaypointData WaypointInfo[18];
    long WaypointCounter;
    void MoveToNextWaypoint(const CScriptThingW& waypoint, CScriptGameResourceObjectScriptedThingBase& guide);
};
void CV_TourGuideScript::MoveToNextWaypoint(const CScriptThingW& waypoint, CScriptGameResourceObjectScriptedThingBase& guide)
{
    if (waypoint.IsAlive()) {
        if (IsActiveThreadTerminating())
            return;
        guide.MoveToPosition(waypoint.GetPos(), 1.0f, MOVE_TYPE_WALK, false, true);
        return;
    }
    if (IsActiveThreadTerminating())
        return;
    CScriptThingW next = waypoint;
    long start = WaypointCounter;
    while (!next.IsAlive()) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        ++WaypointCounter;
        if (WaypointCounter >= 18) {
            if (IsActiveThreadTerminating())
                return;
            WaypointCounter = 0;
        } else if (WaypointCounter == start) {
            if (IsActiveThreadTerminating())
                return;
        }
        next = Game->GetThingWithScriptName(WaypointInfo[WaypointCounter].locMarker);
    }
    if (IsActiveThreadTerminating())
        return;
    guide.MoveToPosition(next.GetPos(), 1.0f, MOVE_TYPE_WALK, false, true);
}
int main()
{
    CGameScriptInterface game;
    CV_TourGuideScript script;
    script.Game = &game;
    for (int i = 0; i < 18; ++i) {
        char name[32];
        std::sprintf(name, "WP_%0*d", i + 1, i);           // name length i + 4 gives each marker a distinct X
        script.WaypointInfo[i].locMarker = CCharString(name);
    }
    CScriptGameResourceObjectScriptedThingBase guide;
    g_alive[0] = "HERE";
    Reset(1000); script.WaypointCounter = 5;
    script.MoveToNextWaypoint(CScriptThingW("HERE", 7.0f), guide);
    CHECK("00ee6850", g_log == "move 7 1 0@0;" && script.WaypointCounter == 5);
    g_alive[1] = script.WaypointInfo[1].locMarker.S;
    Reset(1000); script.WaypointCounter = 16;
    script.MoveToNextWaypoint(CScriptThingW("GONE", 9.0f), guide);
    CHECK("00ee6850", g_log == "move 5 1 0@3;" && script.WaypointCounter == 1);
    Reset(2); script.WaypointCounter = 16;
    script.MoveToNextWaypoint(CScriptThingW("GONE", 9.0f), guide);
    CHECK("00ee6850", g_log == "" && script.WaypointCounter == 17 && g_frame == 2);
    std::printf("00ee6850_TEST PASS\n");
    return 0;
}