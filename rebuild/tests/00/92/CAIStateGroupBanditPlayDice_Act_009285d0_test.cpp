// Behaviour test for CAIStateGroup_BanditPlayDice::Act @ 0x009285D0: into animation, timed dice/playing loop, out animation.
#include <cstdio>
#include <cstring>
class CThing { public: int id; };
class CThingCreatureBase : public CThing { public: };
struct CIntelligentPointerThing {
    CThing* PItem;
    CIntelligentPointerThing(CThing* item) : PItem(item) { }
    CThing* GetPItem() const { return PItem; }
};
class CTCDiceGame { public: long NumberOfPlayers; CIntelligentPointerThing CurrentPlayer; CTCDiceGame() : NumberOfPlayers(0), CurrentPlayer(0) { } };
enum EAIActionReturn { AI_ACTION_STOPPING = 1, AI_ACTION_DONE = 2, AI_ACTION_ABORTED = 3, AI_ACTION_RUNNING = 4 };
static long g_frame = 0;
static int g_rand = 0;
static float g_floatRandom = 0.0f;
static const char* g_log[64];
static int g_logCount = 0;
static const char* g_failOn = 0;
static CThing* g_playerDuringPick = 0;
static CTCDiceGame* g_game = 0;
static int my_rand() { return g_rand; }
struct CPlayAnimation { const char* name; CPlayAnimation(CThingCreatureBase*, const char* n) : name(n) { } };
struct CAIStateGroup_BanditPlayDice {
    CThingCreatureBase* creature;
    unsigned long seed;
    unsigned long randomValue;
    CTCDiceGame* DiceGame;
    CThingCreatureBase* GetCreature() const { return creature; }
    unsigned long* GetPWorldSeed() { return &seed; }
    unsigned long Random(unsigned long range) const { return randomValue % range; }
    EAIActionReturn AIAction_PerformActionAndWait(const CPlayAnimation& action, bool)
    {
        g_log[g_logCount++] = action.name;
        if (std::strcmp(action.name, "ST_DICEGAME_PICKING_DICE") == 0)
            g_playerDuringPick = g_game->CurrentPlayer.GetPItem();
        g_frame += 100;
        if (g_failOn && std::strcmp(action.name, g_failOn) == 0)
            return AI_ACTION_ABORTED;
        return AI_ACTION_DONE;
    }
    EAIActionReturn Act();
};
static float GFFloatRandom(float range, unsigned long*) { return g_floatRandom * range; }
static long GetWorldFrame() { return g_frame; }

EAIActionReturn CAIStateGroup_BanditPlayDice::Act()
{
    EAIActionReturn result = AIAction_PerformActionAndWait(CPlayAnimation(GetCreature(), "ST_DICEGAME_WATCHING_INTO"), false);
    if (result != AI_ACTION_DONE)
        return result;
    unsigned long wait = Random(300);
    long endFrame = GetWorldFrame() + wait + 375;
    while (GetWorldFrame() < endFrame) {
        if (DiceGame->NumberOfPlayers >= 2 && !DiceGame->CurrentPlayer.GetPItem()
            && GFFloatRandom(1.0f, GetPWorldSeed()) < 0.2f) {
            CThing* player = GetCreature();
            CTCDiceGame* game = DiceGame;
            game->CurrentPlayer = CIntelligentPointerThing(player);
            AIAction_PerformActionAndWait(CPlayAnimation(GetCreature(), "ST_DICEGAME_PICKING_DICE"), false);
            result = AIAction_PerformActionAndWait(CPlayAnimation(GetCreature(), "ST_DICEGAME_PLAYING_THROW"), false);
            switch (my_rand() % 5) {
            case 0:
                result = AIAction_PerformActionAndWait(CPlayAnimation(GetCreature(), "ST_DICEGAME_PLAYING_WIN"), false);
                break;
            case 1:
                result = AIAction_PerformActionAndWait(CPlayAnimation(GetCreature(), "ST_DICEGAME_PLAYING_LOSE"), false);
                break;
            }
            CTCDiceGame* table = DiceGame;
            table->CurrentPlayer = CIntelligentPointerThing(0);
        } else
            result = AIAction_PerformActionAndWait(CPlayAnimation(GetCreature(), "ST_DICEGAME_PLAYING"), false);
        if (result != AI_ACTION_DONE)
            return result;
    }
    return AIAction_PerformActionAndWait(CPlayAnimation(GetCreature(), "ST_DICEGAME_WATCHING_OUTOF"), false);
}

static int failures = 0;
static void Check(bool ok, const char* what) { if (!ok) { std::printf("FAIL %s\n", what); ++failures; } }
static bool Logged(int i, const char* name) { return i < g_logCount && std::strcmp(g_log[i], name) == 0; }

int main()
{
    CThingCreatureBase me; me.id = 7;
    CTCDiceGame game; g_game = &game;
    CAIStateGroup_BanditPlayDice g; g.creature = &me; g.seed = 1; g.DiceGame = &game;

    // One player only: into, playing x4 (frames 100..500 < 0+0+375 boundary), out.
    g.randomValue = 0; g_frame = 0; g_logCount = 0; game.NumberOfPlayers = 1;
    Check(g.Act() == AI_ACTION_DONE, "plain run done");
    Check(Logged(0, "ST_DICEGAME_WATCHING_INTO") && Logged(1, "ST_DICEGAME_PLAYING"), "into then playing");
    Check(g_logCount == 6 && Logged(5, "ST_DICEGAME_WATCHING_OUTOF"), "four plays then out");

    // Two players, low float roll, rand 0: pick, throw, win; turn released.
    g_frame = 0; g_logCount = 0; game.NumberOfPlayers = 2; g_floatRandom = 0.1f; g_rand = 5; g.randomValue = 0;
    g_playerDuringPick = 0;
    Check(g.Act() == AI_ACTION_DONE, "dice run done");
    Check(Logged(1, "ST_DICEGAME_PICKING_DICE") && Logged(2, "ST_DICEGAME_PLAYING_THROW") && Logged(3, "ST_DICEGAME_PLAYING_WIN"), "win sequence");
    Check(g_playerDuringPick == &me && game.CurrentPlayer.GetPItem() == 0, "turn taken and released");

    // rand 1: lose; rand 3: no extra animation.
    g_frame = 0; g_logCount = 0; g_rand = 6;
    g.Act();
    Check(Logged(3, "ST_DICEGAME_PLAYING_LOSE"), "lose sequence");
    g_frame = 0; g_logCount = 0; g_rand = 3;
    g.Act();
    Check(Logged(2, "ST_DICEGAME_PLAYING_THROW") && Logged(3, "ST_DICEGAME_PICKING_DICE"), "no result animation");

    // Someone already holds the dice or high roll: just playing.
    g_frame = 0; g_logCount = 0; g_floatRandom = 0.5f;
    g.Act();
    Check(Logged(1, "ST_DICEGAME_PLAYING"), "high roll plays");

    // Failure propagates.
    g_frame = 0; g_logCount = 0; g_failOn = "ST_DICEGAME_PLAYING";
    Check(g.Act() == AI_ACTION_ABORTED && g_logCount == 2, "playing failure returned");
    g_frame = 0; g_logCount = 0; g_failOn = "ST_DICEGAME_WATCHING_INTO";
    Check(g.Act() == AI_ACTION_ABORTED && g_logCount == 1, "into failure returned");
    g_failOn = 0;

    // Random wait extends the loop: end 100 + 299 + 375 = 774 -> plays at 100..700 (7 plays + into + out).
    g_frame = 0; g_logCount = 0; g.randomValue = 299;
    g.Act();
    Check(g_logCount == 9, "longer wait");

    if (failures == 0)
        std::printf("009285d0_TEST PASS\n");
    return failures;
}