// Behaviour test for 0x00E1F0C0 NScript::CQR_EscortTraderScript::DoMission: when the trader is not yet created, wait for
// the LookoutPoint region, take one frame, create CREATURE_TRADER_01 named TraderToEscort at the M_ET_TraderSpawnPoint
// marker's position (the zero vector when the marker is missing) and set TraderCreated; then (also when already created)
// spawn WatchForFailure, run EscortTrader and take a final frame; termination while waiting returns early.
#include <stdio.h>
#include <string.h>

static char g_log[512];
static void Log(const char* s) { strcat(g_log, s); strcat(g_log, ";"); }
static int g_frame, g_regionFrame, g_endFrame;
static float g_createX;
class C3DVector { public: float X, Y, Z; };
static const C3DVector g_ZeroVector = { 0.0f, 0.0f, 0.0f };
struct CImp { C3DVector Pos; };
class CThing { public:
    CImp* Data;
    const C3DVector& GetPos() const { if (!Data) return g_ZeroVector; return Data->Pos; }
};
static CImp g_marker;
static bool g_markerExists;
class CGame { public:
    void NewScriptFrame() { ++g_frame; Log("Frame"); }
    bool IsRegionLoaded(const char* r) { return !strcmp(r, "LookoutPoint") && g_frame >= g_regionFrame; }
    CThing GetThingWithScriptName(const char* n) { Log(n); CThing t; t.Data = g_markerExists ? &g_marker : 0; return t; }
    CThing CreateCreature(const char* def, const C3DVector& pos, const char* name, bool b)
    { Log(def); Log(name); if (b) Log("FlagBad"); g_createX = pos.X; CThing t; t.Data = 0; return t; }
};
struct CEscortTrader {
    CGame* Game;
    bool TraderCreated;
    bool IsActiveThreadTerminating() { return g_frame >= g_endFrame; }
    void SpawnFunction(const char* name, const char* region) { Log(name); Log(region); }
    void EscortTrader() { Log("Escort"); }
    void DoMission()
    {
        if (!TraderCreated) {
            if (IsActiveThreadTerminating())
                return;
            while (!Game->IsRegionLoaded("LookoutPoint")) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
            CThing spawnPoint = Game->GetThingWithScriptName("M_ET_TraderSpawnPoint");
            Game->CreateCreature("CREATURE_TRADER_01", spawnPoint.GetPos(), "TraderToEscort", false);
            TraderCreated = true;
        }
        SpawnFunction("WatchForFailure", "");
        EscortTrader();
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
};

int main()
{
    CGame g; CEscortTrader s; s.Game = &g;
    g_marker.Pos.X = 12.5f; g_marker.Pos.Y = 1.0f; g_marker.Pos.Z = 2.0f;
    s.TraderCreated = false; g_markerExists = true; g_log[0] = 0; g_frame = 0; g_regionFrame = 2; g_endFrame = 100; g_createX = -1.0f;
    s.DoMission();
    int ok = !strcmp(g_log, "Frame;Frame;Frame;M_ET_TraderSpawnPoint;CREATURE_TRADER_01;TraderToEscort;WatchForFailure;;Escort;Frame;")
        && s.TraderCreated && g_createX == 12.5f;
    s.TraderCreated = false; g_markerExists = false; g_log[0] = 0; g_frame = 0; g_regionFrame = 0; g_createX = -1.0f;
    s.DoMission();
    ok = ok && s.TraderCreated && g_createX == 0.0f && g_frame == 2;
    g_log[0] = 0; g_frame = 0;
    s.DoMission();
    ok = ok && !strcmp(g_log, "WatchForFailure;;Escort;Frame;");
    s.TraderCreated = false; g_log[0] = 0; g_frame = 0; g_regionFrame = 100; g_endFrame = 3;
    s.DoMission();
    ok = ok && !s.TraderCreated && strstr(g_log, "Escort") == 0 && g_frame == 3;
    if (!ok) { printf("00e1f0c0_TEST FAIL\n"); return 1; }
    printf("00e1f0c0_TEST PASS\n");
    return 0;
}