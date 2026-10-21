// Behaviour test for 0x00E82650 (CV_FisticuffsClubScript::InitialiseVariables): the reset values and region names.
#include <cstdio>
#include <cstring>
#include <string>
struct Game {
    std::string log; bool nightOnly; long timerSet[8];
    Game(bool n) : nightOnly(n) { for (int i = 0; i < 8; ++i) timerSet[i] = -1; }
    bool IsQuestActive(const std::string& q) { return q == "V_FisticuffsClubOnlyAtNight" && nightOnly; }
    void SetTimer(long t, long v) { timerSet[t] = v; }
};
static Game* g_game; static long g_delay = 42;
struct Script {
    Game* GSI; int termAfter; int checks; std::string calls;
    bool FighterOneInRing, FighterTwoInRing, ChallengeAccepted, FightFinished, HeroInsideRing, HeroOffered, HeroPaid,
         HeroInClub, TylerInFight, HeroBeatTyler, TimeOut, RingCleared, RoundStarted, EverybodyAttack, LostWhileOutside,
         FisticuffsActive, PlayerLost;
    long T1, T2, HitID, HitCount, AttackWarnings, AllowedHits; float CheerNumber, BooNumber; std::string RegionNames[5];
    Script(Game* g, int after) : GSI(g), termAfter(after), checks(0), T1(1), T2(2), HitID(7), HitCount(7), AttackWarnings(7),
        AllowedHits(7), CheerNumber(3), BooNumber(3)
    {
        FighterOneInRing = FighterTwoInRing = ChallengeAccepted = HeroInsideRing = HeroOffered = HeroPaid = HeroInClub =
        TylerInFight = HeroBeatTyler = TimeOut = RoundStarted = EverybodyAttack = LostWhileOutside = PlayerLost = true;
        FightFinished = RingCleared = false; FisticuffsActive = false;
    }
    bool IsActiveThreadTerminating() { return ++checks > termAfter; }
    void SetFightCount() { calls += "S"; }
    void InitialiseVariablesCheck() { calls += "C"; }
    void InitialiseVariables()
    {
        FighterOneInRing = false; FighterTwoInRing = false; ChallengeAccepted = false; FightFinished = true;
        HeroOffered = false; HeroPaid = false; HeroInsideRing = false; HeroInClub = false;
        TylerInFight = false; HeroBeatTyler = false; RingCleared = true; RoundStarted = false;
        GSI->SetTimer(T1, g_delay); GSI->SetTimer(T2, g_delay);
        CheerNumber = 0; BooNumber = 0; TimeOut = false; HitID = 0; PlayerLost = false; HitCount = 0;
        SetFightCount(); InitialiseVariablesCheck();
        AttackWarnings = 0; EverybodyAttack = false; LostWhileOutside = false;
        bool onlyAtNight = GSI->IsQuestActive("V_FisticuffsClubOnlyAtNight");
        if (onlyAtNight) { if (IsActiveThreadTerminating()) return; FisticuffsActive = true; }
        else { if (IsActiveThreadTerminating()) return; FisticuffsActive = false; }
        AllowedHits = 0;
        RegionNames[1] = "BowerstoneSlums"; RegionNames[2] = "Oakbay"; RegionNames[3] = "BanditCampCentre"; RegionNames[4] = "KnotholeGlade";
    }
};
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d: %s\n", __LINE__, #c); } } while (0)
int main()
{
    { Game g(true); Script s(&g, 5); s.InitialiseVariables();
      CHECK(!s.FighterOneInRing && !s.FighterTwoInRing && !s.ChallengeAccepted && s.FightFinished && !s.HeroInsideRing);
      CHECK(!s.HeroOffered && !s.HeroPaid && !s.HeroInClub && !s.TylerInFight && !s.HeroBeatTyler && s.RingCleared && !s.RoundStarted);
      CHECK(g.timerSet[1] == 42 && g.timerSet[2] == 42 && s.CheerNumber == 0 && s.BooNumber == 0 && !s.TimeOut);
      CHECK(s.HitID == 0 && s.HitCount == 0 && !s.PlayerLost && s.calls == "SC" && s.AttackWarnings == 0);
      CHECK(!s.EverybodyAttack && !s.LostWhileOutside && s.FisticuffsActive && s.AllowedHits == 0);
      CHECK(s.RegionNames[0].empty() && s.RegionNames[1] == "BowerstoneSlums" && s.RegionNames[2] == "Oakbay");
      CHECK(s.RegionNames[3] == "BanditCampCentre" && s.RegionNames[4] == "KnotholeGlade"); }
    { Game g(false); Script s(&g, 5); s.FisticuffsActive = true; s.InitialiseVariables(); CHECK(!s.FisticuffsActive && s.AllowedHits == 0); }
    { Game g(true); Script s(&g, 0); s.InitialiseVariables(); CHECK(!s.FisticuffsActive && s.AllowedHits == 7 && s.RegionNames[1].empty() && s.calls == "SC"); }
    if (g_fail) { std::printf("00e82650_TEST FAIL\n"); return 1; }
    std::printf("00e82650_TEST PASS\n");
    return 0;
}
