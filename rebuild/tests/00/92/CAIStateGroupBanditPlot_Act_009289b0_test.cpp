// Behaviour test for CAIStateGroup_BanditPlot::Act (0x009289B0): a copy of the row's logic over a small model of the
// dice-game table (players, current player), the frame clock, the world seed and the animation actions.
#include <cstdio>
#include <cstring>

enum EAIActionReturn { AI_ACTION_STOPPING = 1, AI_ACTION_DONE = 2, AI_ACTION_ABORTED = 3, AI_ACTION_RUNNING = 4 };
class CThing { public: int id; };
class CTCDiceGame { public: long NumberOfPlayers; CThing* CurrentPlayer; };

static long g_frame;
static int g_rand;
static float g_floatRandom;
static const char* g_log[64];
static int g_logCount;
static const char* g_failOn;
static CThing* g_playerDuringPlan;
static CTCDiceGame* g_game;

struct BanditPlot {
    CThing* creature;
    unsigned long seed;
    CTCDiceGame* DiceGame;
    unsigned long Random(unsigned long range)
    {
        seed = seed * 0x24a1 + 0x24df;
        seed = (seed >> 13) | (seed << 19);
        return seed % range;
    }
    EAIActionReturn Play(const char* name)
    {
        if (g_logCount < 64) g_log[g_logCount++] = name;
        if (std::strcmp(name, "ST_MAKING_PLANS_ON_FLOOR_WITH_SWORD_INTO") == 0) g_playerDuringPlan = g_game->CurrentPlayer;
        g_frame += 100;
        return (g_failOn && std::strcmp(name, g_failOn) == 0) ? AI_ACTION_ABORTED : AI_ACTION_DONE;
    }
    EAIActionReturn Act()
    {
        EAIActionReturn result = Play("ST_LISTENING_TO_PLANS_INTO");
        if (result != AI_ACTION_DONE)
            return result;
        unsigned long wait = Random(300);
        long endFrame = g_frame + (long)wait + 375;
        while (g_frame < endFrame) {
            if (DiceGame->NumberOfPlayers >= 2 && !DiceGame->CurrentPlayer && g_floatRandom < 0.2f) {
                DiceGame->CurrentPlayer = creature;
                Play("ST_LISTENING_TO_PLANS_OUTOF");
                Play("ST_MAKING_PLANS_ON_FLOOR_WITH_SWORD_INTO");
                for (long i = 0; i < 2; ++i)
                    Play("ST_MAKING_PLANS_ON_FLOOR_WITH_SWORD_LOOP");
                Play("ST_MAKING_PLANS_ON_FLOOR_WITH_SWORD_OUTOF");
                result = Play("ST_LISTENING_TO_PLANS_INTO");
                DiceGame->CurrentPlayer = 0;
            } else if (g_rand % 10 == 0)
                result = Play("ST_LISTENING_TO_PLANS_BREAKDOWN");
            else
                result = Play("ST_LISTENING_TO_PLANS_LOOP");
            if (result != AI_ACTION_DONE)
                return result;
        }
        return Play("ST_LISTENING_TO_PLANS_OUTOF");
    }
};

static int fail(int n) { std::printf("009289b0_TEST FAIL %d\n", n); return 1; }
static int count(const char* name) { int n = 0; for (int i = 0; i < g_logCount; ++i) if (std::strcmp(g_log[i], name) == 0) ++n; return n; }

int main()
{
    CThing bandit = { 7 };
    CTCDiceGame game = { 1, 0 };
    g_game = &game;
    BanditPlot ai;
    ai.creature = &bandit;
    ai.seed = 12345;
    ai.DiceGame = &game;

    // one player: no plan, loop animations (rand 3 -> plain loop) until the window closes, then the out animation
    g_frame = 0; g_logCount = 0; g_rand = 3; g_floatRandom = 0.0f; g_failOn = 0;
    if (ai.Act() != AI_ACTION_DONE) return fail(1);
    if (count("ST_LISTENING_TO_PLANS_LOOP") < 4 || count("ST_LISTENING_TO_PLANS_BREAKDOWN") || count("ST_MAKING_PLANS_ON_FLOOR_WITH_SWORD_LOOP"))
        return fail(2);
    if (std::strcmp(g_log[g_logCount - 1], "ST_LISTENING_TO_PLANS_OUTOF") != 0) return fail(3);

    // two players, free leader slot, lucky roll: take the lead, act out the plan (two sword loops per plan), release it
    game.NumberOfPlayers = 2;
    g_frame = 0; g_logCount = 0; g_playerDuringPlan = 0;
    if (ai.Act() != AI_ACTION_DONE) return fail(4);
    if (g_playerDuringPlan != &bandit || game.CurrentPlayer != 0) return fail(5);
    if (count("ST_MAKING_PLANS_ON_FLOOR_WITH_SWORD_LOOP") != 2 * count("ST_MAKING_PLANS_ON_FLOOR_WITH_SWORD_INTO")) return fail(6);

    // someone else leads, rand 20 -> breakdowns; a failing breakdown is returned at once
    game.CurrentPlayer = &bandit;
    g_frame = 0; g_logCount = 0; g_rand = 20; g_failOn = "ST_LISTENING_TO_PLANS_BREAKDOWN";
    if (ai.Act() != AI_ACTION_ABORTED) return fail(7);
    if (g_logCount != 2 || count("ST_LISTENING_TO_PLANS_OUTOF")) return fail(8);

    // the opening animation failing ends the act straight away
    g_frame = 0; g_logCount = 0; g_failOn = "ST_LISTENING_TO_PLANS_INTO";
    if (ai.Act() != AI_ACTION_ABORTED || g_logCount != 1) return fail(9);

    std::printf("009289b0_TEST PASS\n");
    return 0;
}