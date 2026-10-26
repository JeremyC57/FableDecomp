#include <cstdio>
#include <string>
#include <vector>
// behaviour test: CPrisonRacer::Main -- after the race starts the racer runs at base speed minus 0.02 per earlier racer;
// it tracks the nearest speed-check waypoint every fourth frame, sprints (SPRINT) while behind the hero's nearest race
// marker until the sprint timer expires (then SLOW), re-follows the route when idle, flags RaceEnded near the warden and
// stops once the hero won
static int g_frames = 0, g_endAt = 1000, g_heroWinsAt = 20;
static std::vector<std::string> g_log;
static std::vector<float> g_speeds;
static long g_timers[2] = { 0, 0 };
static int g_nextTimer = 0;
class CScriptThing { public: std::string data; float x; bool alive; std::string GetDataString() const { return data; } bool IsAlive() const { return alive; } };
struct CHandle { int busy; int routes; CHandle() : busy(0), routes(0) {} bool IsPerformingScriptTask() const { return busy > 0; }
    void FollowPreCalculatedRoute(const CScriptThing&, int, bool, bool) { busy = 3; ++routes; } void ClearCommands() { g_log.push_back("clear"); } };
struct CQ_PrisonRaceScript {
    bool HeroWonRace, RaceStarted, RaceEnded; long RacerSpeed, NearestRaceMarker;
    bool MakePrisonerComment(const CScriptThing&, const std::string& line) { g_log.push_back(line); return true; }
};
static CHandle* g_racer = 0;
static CQ_PrisonRaceScript* g_race = 0;
class CGameScriptInterface { public:
    CScriptThing marker, warden, checks[2];
    void NewScriptFrame() { ++g_frames; for (int i = 0; i < 2; ++i) if (g_timers[i] > 0) --g_timers[i]; if (g_racer && g_racer->busy) --g_racer->busy;
        if (g_frames >= g_heroWinsAt) g_race->HeroWonRace = true; }
    bool StartScriptingEntity(const CScriptThing&, CHandle& h) { g_racer = &h; return true; }
    CScriptThing GetThingWithScriptName(const char*) const { return warden; }
    CScriptThing GetNearestWithScriptName(const CScriptThing&, const std::string& n) const { return n == "RaceMarker" ? marker : checks[1]; }
    long GetAllThingsWithScriptName(const char*, std::vector<CScriptThing>& v) const { v.push_back(checks[0]); v.push_back(checks[1]); return 2; }
    void EntitySetMaxRunningSpeed(const CScriptThing&, float s) { g_speeds.push_back(s); }
    long GetWorldFrame() const { return g_frames; }
};
static float GetSquaredDistanceBetweenThings(const CScriptThing& a, const CScriptThing& b) { return (a.x - b.x) * (a.x - b.x); }
static bool IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float d) { return GetSquaredDistanceBetweenThings(a, b) < d * d; }
static long ToInt(const std::string& s) { return std::atoi(s.c_str()); }
struct CPrisonRacer {
    CGameScriptInterface* Game; CScriptThing Me; CQ_PrisonRaceScript* ParentClass; long RaceWaypointIndex;
    bool IsActiveThreadTerminating() const { return g_frames >= g_endAt; }
    void Main();
};
#include <cstdlib>
void CPrisonRacer::Main()
{
    CHandle racer;
    Game->StartScriptingEntity(Me, racer);
    Game->EntitySetMaxRunningSpeed(Me, 10.0f - ParentClass->RacerSpeed * 0.02f);
    ParentClass->RacerSpeed++;
    CScriptThing warden = Game->GetThingWithScriptName("PrisonRaceWarden");
    bool sprinting = false;
    int sprintTimer = g_nextTimer++, restTimer = g_nextTimer++;
    g_timers[restTimer] = 5;
    CScriptThing marker = Game->GetNearestWithScriptName(Me, "RaceMarker");
    std::vector<CScriptThing> speedChecks;
    Game->GetAllThingsWithScriptName("WaypointSpeedCheck", speedChecks);
    for (;;) {
        if (IsActiveThreadTerminating()) return;
        if (Game->GetWorldFrame() % 4 == 0) {
            float nearest = 1000.0f;
            for (unsigned long i = 0; i < speedChecks.size(); ++i) {
                float d = GetSquaredDistanceBetweenThings(Me, speedChecks[i]);
                if (d < nearest) { nearest = d; RaceWaypointIndex = ToInt(speedChecks[i].GetDataString()); }
            }
        }
        if (!sprinting && ParentClass->NearestRaceMarker > RaceWaypointIndex) {
            sprinting = true; g_timers[sprintTimer] = 4;
            Game->EntitySetMaxRunningSpeed(Me, 12.0f - ParentClass->RacerSpeed * 0.02f);
            ParentClass->MakePrisonerComment(Me, "SPRINT");
        }
        if (sprinting && g_timers[sprintTimer] == 0) {
            sprinting = false; g_timers[restTimer] = 6;
            Game->EntitySetMaxRunningSpeed(Me, 10.0f - ParentClass->RacerSpeed * 0.02f);
            ParentClass->MakePrisonerComment(Me, "SLOW");
        }
        if (marker.IsAlive() && !racer.IsPerformingScriptTask()) {
            marker = Game->GetNearestWithScriptName(Me, "RaceMarker");
            CScriptThing speedCheck = Game->GetNearestWithScriptName(Me, "WaypointSpeedCheck");
            RaceWaypointIndex = ToInt(speedCheck.GetDataString());
            racer.FollowPreCalculatedRoute(marker, 1, false, true);
        }
        if (ParentClass->HeroWonRace) { racer.ClearCommands(); do { Game->NewScriptFrame(); } while (!IsActiveThreadTerminating()); return; }
        if (IsDistanceBetweenThingsUnder(warden, Me, 5.0f)) ParentClass->RaceEnded = true;
        Game->NewScriptFrame();
    }
}
int main()
{
    CQ_PrisonRaceScript race; race.HeroWonRace = false; race.RaceStarted = true; race.RaceEnded = false; race.RacerSpeed = 2; race.NearestRaceMarker = 5;
    g_race = &race;
    CGameScriptInterface game;
    game.marker.alive = true; game.marker.x = 50; game.warden.x = 3; game.warden.alive = true;
    game.checks[0].data = "1"; game.checks[0].x = 0; game.checks[0].alive = true;
    game.checks[1].data = "7"; game.checks[1].x = 100; game.checks[1].alive = true;
    CPrisonRacer r; r.Game = &game; r.ParentClass = &race; r.RaceWaypointIndex = 0; r.Me.x = 0; r.Me.alive = true;
    r.Main();
    // frame 0: nearest check index 1 (< marker 5) -> SPRINT; route follow sets index 7 from the nearest check
    // sprint timer 4 frames -> SLOW at frame 4, then the world-frame scan resets the index to 1 -> SPRINT again
    bool ok = race.RacerSpeed == 3 && race.RaceEnded && race.HeroWonRace && g_log.size() >= 3 && g_log[0] == "SPRINT" && g_log[1] == "SLOW"
        && g_log.back() == "clear" && g_speeds.size() >= 2 && g_speeds[0] > 9.95f && g_speeds[0] < 9.97f && g_frames == 1000;
    if (!ok) { std::printf("00ddc980_TEST FAIL\n"); return 1; }
    std::printf("00ddc980_TEST PASS\n");
    return 0;
}