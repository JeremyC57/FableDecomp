#include <cstdio>
#include <cstring>
static char g_log[8192];
static int g_frame = 0;
static int g_termAt = 1000;
static void Log(const char* s) { char b[32]; std::sprintf(b, "@%d;", g_frame); std::strcat(g_log, s); std::strcat(g_log, b); }
static void Reset(int termAt) { g_log[0] = 0; g_frame = 0; g_termAt = termAt; }
static int Fail(const char* addr, const char* what) { std::printf("%s_TEST FAIL %s [%s]\n", addr, what, g_log); return 1; }
#define CHECK(addr, cond) do { if (!(cond)) return Fail(addr, #cond); } while (0)
// behaviour test: CBountyHuntActivationChecker::Main branches on the bounty-hunt/competition state and catches up the attack counter
struct C3DVector { float X, Y, Z; };
static bool g_huntActive = false;
static long g_day = 10;
struct CScriptThing {
    const char* Name;
    CScriptThing(const char* n = "") : Name(n) { }
    C3DVector GetPos() const { C3DVector v; v.X = 5.0f; v.Y = v.Z = 0.0f; return v; }
};
struct CGame {
    CScriptThing GetThingWithScriptName(const char* n) const { return CScriptThing(n); }
    CScriptThing CreateCreature(const char* def, const C3DVector& p, const char* name, bool) const
    { char x[96]; std::sprintf(x, "create %s %s %g", def, name, p.X); Log(x); return CScriptThing(name); }
    bool IsQuestActive(const char*) const { return g_huntActive; }
    long GetDayCount() const { return g_day; }
    void SetReadableObjectTextTag(const CScriptThing& t, const char* tag) const { char x[96]; std::sprintf(x, "tag %s %s", t.Name, tag); Log(x); }
};
struct CV_FishingCompetitionScript {
    bool BountyHuntActive, OrganiserRanAway, CompetitionRunning, CompetitionWon, CommentIRanAway;
    long AttackCounter, AttackDayCount, HitCount;
    void GenerateCompetitionEntry() { Log("entry"); }
    void RefreshCompetition() { Log("refresh"); }
};
struct CBountyHuntActivationChecker {
    CGame* Game; CV_FishingCompetitionScript* ParentClass;
    bool IsActiveThreadTerminating() const { return g_frame >= g_termAt; }
    void RespawnFishermen() { Log("respawn"); }
    void RemoveFishermen() { Log("removefish"); }
    void Main();
};
void CBountyHuntActivationChecker::Main()
{
    CScriptThing sign = Game->GetThingWithScriptName("FishCompSign");
    if (Game->IsQuestActive("Q_BountyHunt")) {
        if (IsActiveThreadTerminating())
            return;
        ParentClass->BountyHuntActive = true;
        RemoveFishermen();
        if (!ParentClass->CompetitionWon) {
            if (IsActiveThreadTerminating())
                return;
            Game->SetReadableObjectTextTag(sign, "TEXT_QST_B15_FISH_SIGN_INFO_BANDITS");
        }
    } else {
        bool huntEnded = !Game->IsQuestActive("Q_BountyHunt") && ParentClass->BountyHuntActive;
        if (huntEnded) {
            if (IsActiveThreadTerminating())
                return;
            ParentClass->BountyHuntActive = false;
            RespawnFishermen();
            if (!ParentClass->CompetitionWon) {
                if (IsActiveThreadTerminating())
                    return;
                ParentClass->RefreshCompetition();
            }
        } else if (ParentClass->CompetitionWon) {
            if (IsActiveThreadTerminating())
                return;
            ParentClass->RefreshCompetition();
            return;
        } else if (!ParentClass->CompetitionRunning) {
            if (IsActiveThreadTerminating())
                return;
            Game->SetReadableObjectTextTag(sign, "TEXT_QST_B15_FISH_SIGN_INFO_RANAWAY");
        } else if (ParentClass->OrganiserRanAway) {
            if (IsActiveThreadTerminating())
                return;
            Game->CreateCreature("CREATURE_FISHERMAN", Game->GetThingWithScriptName("MK_FC_ORG").GetPos(), "FishCompOrganiser", false);
            ParentClass->RefreshCompetition();
            ParentClass->OrganiserRanAway = false;
            ParentClass->CommentIRanAway = true;
        } else {
            if (IsActiveThreadTerminating())
                return;
            ParentClass->GenerateCompetitionEntry();
            ParentClass->RefreshCompetition();
        }
    }
    if (ParentClass->AttackCounter > 0) {
        if (IsActiveThreadTerminating())
            return;
        long day = Game->GetDayCount();
        while (ParentClass->AttackCounter > 0 && day > ParentClass->AttackDayCount) {
            ParentClass->AttackDayCount++;
            ParentClass->AttackCounter--;
            ParentClass->HitCount--;
        }
    }
}
static CV_FishingCompetitionScript P(bool hunt, bool ran, bool running, bool won)
{ CV_FishingCompetitionScript p; p.BountyHuntActive = hunt; p.OrganiserRanAway = ran; p.CompetitionRunning = running; p.CompetitionWon = won; p.CommentIRanAway = false; p.AttackCounter = 0; p.AttackDayCount = 0; p.HitCount = 0; return p; }
int main()
{
    CGame game; CBountyHuntActivationChecker c; c.Game = &game;
    CV_FishingCompetitionScript p = P(false, false, true, false); c.ParentClass = &p;
    Reset(100); g_huntActive = true; c.Main();
    CHECK("00e7f410", std::strcmp(g_log, "removefish@0;tag FishCompSign TEXT_QST_B15_FISH_SIGN_INFO_BANDITS@0;") == 0 && p.BountyHuntActive);
    Reset(100); g_huntActive = false; c.Main();
    CHECK("00e7f410", std::strcmp(g_log, "respawn@0;refresh@0;") == 0 && !p.BountyHuntActive);
    Reset(100); p = P(false, false, true, true); p.AttackCounter = 5; c.Main();
    CHECK("00e7f410", std::strcmp(g_log, "refresh@0;") == 0 && p.AttackCounter == 5);
    Reset(100); p = P(false, false, false, false); c.Main();
    CHECK("00e7f410", std::strcmp(g_log, "tag FishCompSign TEXT_QST_B15_FISH_SIGN_INFO_RANAWAY@0;") == 0);
    Reset(100); p = P(false, true, true, false); p.AttackCounter = 5; p.AttackDayCount = 7; p.HitCount = 9; g_day = 10; c.Main();
    CHECK("00e7f410", std::strcmp(g_log, "create CREATURE_FISHERMAN FishCompOrganiser 5@0;refresh@0;") == 0 && !p.OrganiserRanAway && p.CommentIRanAway);
    CHECK("00e7f410", p.AttackCounter == 2 && p.AttackDayCount == 10 && p.HitCount == 6);
    Reset(100); p = P(false, false, true, false); c.Main();
    CHECK("00e7f410", std::strcmp(g_log, "entry@0;refresh@0;") == 0);
    Reset(0); p = P(false, false, true, false); c.Main();
    CHECK("00e7f410", g_log[0] == 0);
    std::printf("00e7f410_TEST PASS\n");
    return 0;
}
