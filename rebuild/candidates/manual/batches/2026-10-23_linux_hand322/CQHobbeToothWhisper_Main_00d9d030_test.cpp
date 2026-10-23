#include <cstdio>
#include <cstring>
// behaviour test: CQ_HobbeToothWhisper::Main -- faction switch on a hero hit until the contest ends, then hunt loop:
// nearest hobbe (fallback drinking hobbe) -> enemy + target + move + drain while alive, else walk to the spawn point
static char g_log[2048];
static void L(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
static int g_frames = 0, g_endAt = 9, g_hobbeHealth = 0;
struct CContest { bool ContestOver; bool WhisperHit; };
static CContest g_contest;
struct CThing { const char* name; bool alive; };
struct CGameScriptInterface {
    void NewScriptFrame() const { ++g_frames; if (g_frames == 2) g_contest.ContestOver = true; }
    bool StartScriptingEntity(int) { return true; }
    CThing GetNearestWithScriptName(const char* n) const { CThing t; t.name = n; t.alive = !std::strcmp(n, "DrinkingHobbe") && g_frames < 4; return t; }
    void ModifyThingHealth(float a, bool) const { g_hobbeHealth -= (int)a; }
    void GiveThingBestEnemyTarget(const CThing& t) const { L(t.name); }
    void EntitySetThingAsEnemyOfThing(const CThing&) const { L("enemy"); }
    void EntitySetInFaction(const char* f) const { L(f); }
};
struct CQ_HobbeToothWhisper {
    CGameScriptInterface* Game; CContest* ParentClass; bool hitOnce;
    bool IsActiveThreadTerminating() { return g_frames >= g_endAt; }
    bool MsgIsHitBy() { bool h = hitOnce; hitOnce = false; return h; }
    void MoveToThing(const CThing& t) { L("move"); L(t.name); }
    void Main()
    {
        while (!ParentClass->ContestOver) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
            bool hit = MsgIsHitBy();
            if (hit) { ParentClass->WhisperHit = true; Game->EntitySetInFaction("FACTION_HERO"); }
        }
        if (IsActiveThreadTerminating()) return;
        while (!Game->StartScriptingEntity(1)) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
        while (!IsActiveThreadTerminating()) {
            {
                CThing hobbe = Game->GetNearestWithScriptName("Q_HobbeToothHobbe");
                if (!hobbe.alive) hobbe = Game->GetNearestWithScriptName("DrinkingHobbe");
                if (hobbe.alive) {
                    Game->EntitySetThingAsEnemyOfThing(hobbe);
                    Game->GiveThingBestEnemyTarget(hobbe);
                    MoveToThing(hobbe);
                    while (hobbe.alive) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; Game->ModifyThingHealth(1.0f, false); if (g_frames >= 4) hobbe.alive = false; }
                    if (IsActiveThreadTerminating()) return;
                } else {
                    CThing spawn = Game->GetNearestWithScriptName("SpawnNewHobbe");
                    while (!Game->StartScriptingEntity(4)) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
                    MoveToThing(spawn);
                }
            }
            Game->NewScriptFrame();
        }
    }
};
int main()
{
    CGameScriptInterface gsi; CQ_HobbeToothWhisper w; w.Game = &gsi; w.ParentClass = &g_contest; w.hitOnce = true;
    g_contest.ContestOver = false; g_contest.WhisperHit = false;
    w.Main();
    const char* want = "FACTION_HERO;enemy;DrinkingHobbe;move;DrinkingHobbe;move;SpawnNewHobbe;move;SpawnNewHobbe;move;SpawnNewHobbe;move;SpawnNewHobbe;";
    if (std::strcmp(g_log, want) != 0 || !g_contest.WhisperHit || g_hobbeHealth != -2 || g_frames != 9) { std::printf("00d9d030_TEST FAIL\n"); return 1; }
    std::printf("00d9d030_TEST PASS\n");
    return 0;
}
