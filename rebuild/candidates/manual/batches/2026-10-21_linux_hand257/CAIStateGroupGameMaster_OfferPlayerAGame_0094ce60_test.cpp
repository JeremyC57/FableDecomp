// Behaviour test for CAIStateGroup_GameMaster::OfferPlayerAGame (0x0094CE60): early outs (no hero, dead/absent table),
// speak loop retried while the action returns 4, Terminate on 1, otherwise next reaction = frame + (rand%10 + 5) * fps.
#include <cstdio>
static unsigned int ror13(unsigned int v) { return (v >> 13) | (v << 19); }
struct Thing { bool dead; int banter; };
struct Group {
    Thing* hero; Thing* table; unsigned int seed; int frame; int fps;
    int speakResults[4]; int speakCalls; bool terminated; int nextTime; int lastBanter; int animations;
    int Speak(int banter) { lastBanter = banter; return speakResults[speakCalls++]; }
    bool OfferPlayerAGame()
    {
        if (hero == 0 || table == 0 || table->dead) return false;
        int banter = table->banter;
        ++animations;
        int result;
        do { result = Speak(banter); } while (result == 4);
        if (result == 1) { terminated = true; return false; }
        seed = seed * 0x24A1 + 0x24DF;
        seed = ror13(seed);
        unsigned int seconds = seed % 10;
        int fps2 = fps;
        nextTime = frame + (int)(seconds + 5) * fps2;
        return true;
    }
};
static void Reset(Group& g, Thing* hero, Thing* table)
{
    g.hero = hero; g.table = table; g.seed = 1234; g.frame = 1000; g.fps = 30;
    g.speakResults[0] = 4; g.speakResults[1] = 4; g.speakResults[2] = 2; g.speakResults[3] = 2;
    g.speakCalls = 0; g.terminated = false; g.nextTime = -1; g.lastBanter = 0; g.animations = 0;
}
int main()
{
    int ok = 1;
    Thing hero = { false, 0 }; Thing table = { false, 77 }; Thing deadTable = { true, 5 };
    Group g;
    Reset(g, 0, &table); ok &= !g.OfferPlayerAGame() && g.animations == 0;
    Reset(g, &hero, 0); ok &= !g.OfferPlayerAGame() && g.animations == 0;
    Reset(g, &hero, &deadTable); ok &= !g.OfferPlayerAGame() && g.speakCalls == 0;
    Reset(g, &hero, &table);
    ok &= g.OfferPlayerAGame() && g.speakCalls == 3 && g.lastBanter == 77 && !g.terminated;
    unsigned int s = ror13(1234u * 0x24A1u + 0x24DFu);
    ok &= g.nextTime == 1000 + (int)(s % 10 + 5) * 30 && g.seed == s;
    ok &= g.nextTime >= 1150 && g.nextTime <= 1420;
    Reset(g, &hero, &table); g.speakResults[0] = 1;
    ok &= !g.OfferPlayerAGame() && g.terminated && g.nextTime == -1;
    std::printf(ok ? "0094ce60_TEST PASS\n" : "0094ce60_TEST FAIL\n");
    return ok ? 0 : 1;
}
