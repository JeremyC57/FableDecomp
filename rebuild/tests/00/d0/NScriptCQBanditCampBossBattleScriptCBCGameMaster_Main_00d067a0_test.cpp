// Behaviour test for CQ_BanditCampBossBattleScript::CBCGameMaster::Main (0x00D067A0): the pass is given to the games master once
// (GivenPass), the intro chat happens once and only while the hero has no pass and gate 2 is shut, and the tavern prize table
// opens when the spot-the-addition game is beaten.
#include <cstdio>

struct GameMaster {
    bool GivenPass, PubGameChatted, HeroHasPass, Gate2Open, PrizeTable; int PassesGiven, Chats;
    void Start()
    {
        if (!GivenPass) { GivenPass = true; ++PassesGiven; }
        if (!PubGameChatted && !HeroHasPass && !Gate2Open) { ++Chats; PubGameChatted = true; }
        PrizeTable = false;
    }
    void Beaten() { PrizeTable = true; }
};

int main()
{
    GameMaster g = { false, false, false, false, false, 0, 0 };
    g.Start();
    g.Start();
    bool ok = g.PassesGiven == 1 && g.Chats == 1 && !g.PrizeTable;
    GameMaster h = { false, false, true, false, false, 0, 0 };
    h.Start();
    ok = ok && h.Chats == 0 && h.PassesGiven == 1;
    g.Beaten();
    ok = ok && g.PrizeTable;
    if (!ok) { std::printf("00d067a0_TEST FAIL\n"); return 1; }
    std::printf("00d067a0_TEST PASS\n");
    return 0;
}