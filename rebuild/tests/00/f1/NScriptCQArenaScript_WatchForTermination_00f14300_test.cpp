// Behaviour test for NScript::CQ_ArenaScript::WatchForTermination (0x00F14300): wait until the arena mission fails or
// succeeds, restart time; on success pay the gold total, give the spare-Whisper morality unless the hero killed Whisper,
// teleport the hero to the arena exterior entrance without a fade, wait for the exterior to load, complete the quest, fade
// in and mark the arena finished; in both cases re-enable teleporting and deactivate the quest.
#include <cstdio>
#include <cstring>

static char gLog[1024];
static void Log(const char* s) { std::strcat(gLog, s); std::strcat(gLog, ";"); }

struct Thing { const char* Name; };
struct Game {
    int frames, endAt, loadedAt; bool* failed; bool* succeeded; bool failMode;
    long gold; float morality; int stopIndex; bool timeStopped;
    Thing hero, entrance;
    void NewScriptFrame() { ++frames; if (frames == endAt) { if (failMode) *failed = true; else *succeeded = true; } }
    void SetTimeAsStopped(bool stop, int index) { timeStopped = stop; stopIndex = index; }
    void GiveHeroGold(long g) { gold += g; }
    void GiveHeroMorality(float m) { morality += m; }
    Thing GetThingWithScriptName(const char* n) { entrance.Name = n; return entrance; }
    void SetAllowScreenFadingOnNextRegionChange(bool on) { Log(on ? "fade+" : "fade-"); }
    Thing& GetHero() { return hero; }
    void EntityTeleportToThing(const Thing& who, const Thing& to, bool) { if (who.Name == hero.Name) { Log("teleport"); Log(to.Name); } }
    bool IsRegionLoaded(const char* r) { return std::strcmp(r, "ArenaExterior") == 0 && frames >= loadedAt; }
    const char* GetActiveQuestName() { return "Q_Arena"; }
    void SetQuestAsCompleted(const char* q, bool a, bool b, bool c) { if (a && !b && !c) { Log("completed"); Log(q); } }
    void FadeScreenIn() { Log("fadein"); }
    void SetTeleportingAsActive(bool on) { Log(on ? "teleporting+" : "teleporting-"); }
    void DeactivateQuestLater(const char* q, unsigned long d) { if (d == 0) { Log("deactivate"); Log(q); } }
};
struct MasterData { bool WhisperKilledByHero, ArenaFinished; };
class CScriptDef { public: float ArenaSpareWhisperMorality; };
static CScriptDef gDef = { 25.0f };
static CScriptDef* pGlobalScriptDef = &gDef;

struct CQ_ArenaScript {
    Game* Game_; MasterData* MasterScriptData;
    bool MissionFailed, MissionSucceeded; long GoldTotal; int StopTimeIndex;
    bool IsActiveThreadTerminating() { return false; }
    void WatchForTermination()
    {
        while (!MissionFailed && !MissionSucceeded) {
            Game_->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        Game_->SetTimeAsStopped(false, StopTimeIndex);
        if (MissionFailed) {
            if (IsActiveThreadTerminating()) return;
        } else {
            if (IsActiveThreadTerminating()) return;
            Game_->GiveHeroGold(GoldTotal);
            if (!MasterScriptData->WhisperKilledByHero) {
                if (IsActiveThreadTerminating()) return;
                Game_->GiveHeroMorality(pGlobalScriptDef->ArenaSpareWhisperMorality);
            }
            Thing entrance = Game_->GetThingWithScriptName("ArenaExtEntrance");
            Game_->SetAllowScreenFadingOnNextRegionChange(false);
            Game_->EntityTeleportToThing(Game_->GetHero(), entrance, false);
            while (!Game_->IsRegionLoaded("ArenaExterior")) {
                Game_->NewScriptFrame();
                if (IsActiveThreadTerminating()) return;
            }
            if (IsActiveThreadTerminating()) return;
            Game_->SetQuestAsCompleted(Game_->GetActiveQuestName(), true, false, false);
            Game_->FadeScreenIn();
            Game_->SetAllowScreenFadingOnNextRegionChange(true);
            MasterScriptData->ArenaFinished = true;
        }
        Game_->SetTeleportingAsActive(true);
        Game_->DeactivateQuestLater(Game_->GetActiveQuestName(), 0);
    }
};

int main()
{
    MasterData m; m.WhisperKilledByHero = false; m.ArenaFinished = false;
    CQ_ArenaScript s; Game g;
    s.Game_ = &g; s.MasterScriptData = &m; s.MissionFailed = false; s.MissionSucceeded = false; s.GoldTotal = 1200; s.StopTimeIndex = 7;
    g.frames = 0; g.endAt = 2; g.loadedAt = 5; g.failed = &s.MissionFailed; g.succeeded = &s.MissionSucceeded; g.failMode = false;
    g.gold = 0; g.morality = 0.0f; g.timeStopped = true; g.hero.Name = "Hero";
    gLog[0] = 0;
    s.WatchForTermination();
    if (g.gold != 1200 || g.morality != 25.0f || g.timeStopped || g.stopIndex != 7 || !m.ArenaFinished || g.frames != 5
        || std::strcmp(gLog, "fade-;teleport;ArenaExtEntrance;completed;Q_Arena;fadein;fade+;teleporting+;deactivate;Q_Arena;") != 0) {
        std::printf("00f14300_TEST FAIL\n"); return 1;
    }
    m.ArenaFinished = false; s.MissionFailed = false; s.MissionSucceeded = false; g.frames = 0; g.failMode = true;
    g.gold = 0; g.morality = 0.0f; gLog[0] = 0;
    s.WatchForTermination();
    if (g.gold != 0 || m.ArenaFinished || std::strcmp(gLog, "teleporting+;deactivate;Q_Arena;") != 0) {
        std::printf("00f14300_TEST FAIL\n"); return 1;
    }
    std::printf("00f14300_TEST PASS\n");
    return 0;
}