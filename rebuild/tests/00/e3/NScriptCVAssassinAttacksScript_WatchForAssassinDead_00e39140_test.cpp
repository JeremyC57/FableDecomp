// behaviour test for NScript::CV_AssassinAttacksScript::WatchForAssassinDead (0x00E39140): a copy of the loop over a mock game
#include <cstdio>
#include <cstring>

struct MockGame {
    int frames, cards, xp;
    const char* region;
    MockGame() : frames(0), cards(0), xp(0), region("REGION_A") {}
    void NewScriptFrame() { ++frames; }
    const char* GetRegionName() { return region; }
    const char* GetActiveQuestName() { return "QUEST"; }
    void GiveHeroQuestCardDirectly(const char*, const char*, bool) { ++cards; }
    void GiveHeroExperience(long n) { xp += n; }
};

struct Assassin {
    int dieAtFrame;                       // frame at which the assassin dies (-1: never)
    const char* killer;                   // killer's script name, 0: killed by nothing (null thing)
    MockGame* game;
    bool IsAlive() const { return dieAtFrame < 0 || game->frames < dieAtFrame; }
    const char* MsgWhoKilledMe() const { return IsAlive() ? 0 : killer; }
};

struct RegionData { int count; bool assassinActive; };

struct Script {
    MockGame* Game;
    int terminateAtFrame;
    bool GivenQuestCard;
    const char* RegionOfMostRecentKill;
    RegionData RegionInfo[2];
    long RegionKillCounts[2];
    long AssassinExperience;
    bool IsActiveThreadTerminating() { return terminateAtFrame >= 0 && Game->frames >= terminateAtFrame; }
    int GetRegionIdForRegion(const char* name) { return std::strcmp(name, "REGION_B") == 0 ? 1 : 0; }
    void WatchForAssassinDead(const Assassin& assassin)
    {
        for (;;) {
            if (IsActiveThreadTerminating()) return;
            const char* killer = assassin.MsgWhoKilledMe();
            if (killer != 0) {
                if (IsActiveThreadTerminating()) return;
                if (!GivenQuestCard) {
                    if (IsActiveThreadTerminating()) return;
                    Game->GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_ASSASSIN_ATTACKS", Game->GetActiveQuestName(), true);
                    GivenQuestCard = true;
                }
                RegionOfMostRecentKill = Game->GetRegionName();
                int region = GetRegionIdForRegion(RegionOfMostRecentKill);
                RegionInfo[region].assassinActive = false;
                if (std::strcmp(killer, "SCRIPT_NAME_HERO") == 0) {
                    if (IsActiveThreadTerminating()) return;
                    RegionKillCounts[region] = ++RegionInfo[region].count;
                    Game->GiveHeroExperience(AssassinExperience);
                }
                break;
            }
            if (!assassin.IsAlive()) {
                if (IsActiveThreadTerminating()) return;
                break;
            }
            Game->NewScriptFrame();
        }
    }
};

static Script MakeScript(MockGame* g, int term)
{
    Script s;
    s.Game = g; s.terminateAtFrame = term; s.GivenQuestCard = false; s.RegionOfMostRecentKill = "";
    for (int i = 0; i < 2; ++i) { s.RegionInfo[i].count = 0; s.RegionInfo[i].assassinActive = true; s.RegionKillCounts[i] = 0; }
    s.AssassinExperience = 250;
    return s;
}

int main()
{
    bool ok = true;
    MockGame g1; g1.region = "REGION_B";
    Script s1 = MakeScript(&g1, -1);
    Assassin a1 = { 4, "SCRIPT_NAME_HERO", &g1 };       // the hero kills the assassin on frame 4
    s1.WatchForAssassinDead(a1);
    ok = ok && g1.frames == 4 && g1.cards == 1 && s1.GivenQuestCard && g1.xp == 250
            && s1.RegionInfo[1].count == 1 && s1.RegionKillCounts[1] == 1 && !s1.RegionInfo[1].assassinActive && s1.RegionInfo[0].assassinActive;
    MockGame g2;
    Script s2 = MakeScript(&g2, -1);
    s2.GivenQuestCard = true;
    Assassin a2 = { 2, "SCRIPT_NAME_GUARD", &g2 };      // someone else kills it: region cleared, no experience
    s2.WatchForAssassinDead(a2);
    ok = ok && g2.cards == 0 && g2.xp == 0 && !s2.RegionInfo[0].assassinActive && s2.RegionInfo[0].count == 0;
    MockGame g3;
    Script s3 = MakeScript(&g3, 3);
    Assassin a3 = { -1, "SCRIPT_NAME_HERO", &g3 };      // never dies: the thread terminates
    s3.WatchForAssassinDead(a3);
    ok = ok && g3.frames == 3 && g3.cards == 0;
    if (!ok) { std::printf("00e39140_TEST FAIL\n"); return 1; }
    std::printf("00e39140_TEST PASS\n");
    return 0;
}