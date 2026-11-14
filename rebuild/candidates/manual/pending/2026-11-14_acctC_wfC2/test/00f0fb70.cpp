// Behaviour test for NScript::CQ_ArenaScript::Main (0x00F0FB70): a state model of the arena quest's main coroutine. Checked: the
// start-up (objective 01, 14 bindings, the holding quest only when inactive, two watchers, the Chicken Chaser title only for title 0),
// the resumable state machine (each MAS_* stage runs only from its own state and hands over to the next), the round loop (the wave-0
// announcement per round: objective, crowd level, cut-scene name, $THEME / $SAY arguments, the $HEROTITLE name of each hero title, the random Whisper line,
// Whisper as an ally from round 4, the final-battle flag), leaving to the cells and coming back (gold multiplier reset, the cell
// Whisper created from round 4 and removed before), the two Lady Grey endings, the exit sequence, and that every termination check
// returns at once with the widescreen / pause object released.
#include <cstdio>
#include <cstring>
#include <string>

enum { MAS_ON_THE_WAY, MAS_INTRO_HALL, MAS_CELLS_PRE_ARENA_INTRO, MAS_CELLS_PRE_ARENA_NORMAL, MAS_CELLS_PRE_ARENA_CHAM_CUTSCENE,
       MAS_CELLS_PRE_ARENA_NORMAL_GO, MAS_IN_ARENA, MAS_CELLS_MID_ARENA, MAS_IN_ARENA_WHISPER };

struct Fields {
    // world model
    int Frame, TerminateAt, LevelDelay, CellStay, WavesPerRound, LeaveAfterRound, StartFailures, HeroTitle, RandPos, Timer;
    int Rands[32];
    bool HoldingActive, CellWhisperAlive, WhisperAllyAlive, WhisperKilledByHero;
    bool WhisperBound;   // the script's own `Whisper` thing: bound only from round 4 on
    // script state
    int ArenaState, ArenaRound, ArenaRoundWave, GoldMultiplier, NewCrowdBaseLevel, Bindings, Watchers, Paused, AutoSaves, Polls, CellFrames;
    bool ChamLeaving, PlayerLeaving, PlayerWon, MissionSucceeded, FinalBattleCS, WhisperNeededForCutscene, MiniMap, ItemsReturned;
};
struct Arena : Fields {
    std::string Log;

    struct Pause { Arena& A; Pause(Arena& a) : A(a) { ++A.Paused; } ~Pause() { --A.Paused; } };
    bool T() const { return Frame >= TerminateAt; }
    void L(const char* s) { Log += s; Log += ' '; }
    void NSF() {
        ++Frame; if (Timer > 0) --Timer;
        if ((ArenaState == MAS_CELLS_PRE_ARENA_NORMAL_GO || ArenaState == MAS_CELLS_MID_ARENA) && ++CellFrames >= CellStay) {
            CellFrames = 0; ArenaState = MAS_IN_ARENA; PlayerLeaving = false;   // the exit guard's script lets the hero out
        }
    }
    bool IsLevelLoaded() { return ++Polls % (LevelDelay + 1) == 0; }
    bool StartScriptingEntity() { if (StartFailures > 0) { --StartFailures; return false; } return true; }
    int Rand() { return Rands[RandPos++ % 32]; }
    void PlayWave() {   // the wave coroutine: here a wave ends at once; the last wave of a round ends the round
        L("wave");
        if (++ArenaRoundWave == WavesPerRound) { ArenaRoundWave = 0; ++ArenaRound; if (ArenaRound == LeaveAfterRound) PlayerLeaving = true; }
    }
#define WAIT_LEVEL while (!IsLevelLoaded()) { NSF(); if (T()) return; } if (T()) return;
#define START while (!StartScriptingEntity()) { NSF(); if (T()) return; } if (T()) return;
#define CELL_WHISPER \
    if (ArenaRound > 2) { if (T()) return; if (!CellWhisperAlive) { if (T()) return; CellWhisperAlive = true; L("+cellwhisper"); } } \
    else if (CellWhisperAlive) { if (T()) return; CellWhisperAlive = false; L("-cellwhisper"); }
    void Main()
    {
        L("obj01"); Bindings = 14;
        if (!HoldingActive) { if (T()) return; HoldingActive = true; L("holding"); }
        Watchers = 2;
        if (HeroTitle == 0) { if (T()) return; HeroTitle = 1; L("chicken"); }
        if (ArenaState == MAS_ON_THE_WAY) { if (T()) return; WAIT_LEVEL ArenaState = MAS_INTRO_HALL; }
        if (ArenaState == MAS_INTRO_HALL) {
            if (T()) return;
            L("CS_ARENA_HOH_INTRO");
            WAIT_LEVEL
            ArenaState = MAS_CELLS_PRE_ARENA_INTRO;
        }
        if (ArenaState == MAS_CELLS_PRE_ARENA_INTRO) {
            if (T()) return;
            { START START START START START START START Pause p(*this); L("CS_ARENA_CELLS_INTRO"); }
            ChamLeaving = true; ArenaState = MAS_CELLS_PRE_ARENA_NORMAL;
        }
        if (ArenaState == MAS_CELLS_PRE_ARENA_NORMAL) {
            if (T()) return;
            L("obj02"); Timer = 60;
            while (Timer > 0) { NSF(); if (T()) return; }
            if (T()) return;
            ArenaState = MAS_CELLS_PRE_ARENA_CHAM_CUTSCENE;
        }
        if (ArenaState == MAS_CELLS_PRE_ARENA_CHAM_CUTSCENE) {
            if (T()) return;
            L("obj03");
            { START START START START START START Pause p(*this); L("CS_ARENA_CELLS_INTRO_CHAMDEAD"); }
            ArenaState = MAS_CELLS_PRE_ARENA_NORMAL_GO;
        }
        if (ArenaState == MAS_CELLS_PRE_ARENA_NORMAL_GO) {
            if (T()) return;
            CELL_WHISPER
            while (ArenaState != MAS_IN_ARENA) { NSF(); if (T()) return; }
            if (T()) return;
            WAIT_LEVEL
            MiniMap = false; PlayerLeaving = false; L("gate");
        }
        if (ArenaState == MAS_CELLS_MID_ARENA) {
            if (T()) return;
            CELL_WHISPER
            while (ArenaState != MAS_IN_ARENA) { NSF(); if (T()) return; }
            if (T()) return;
            WAIT_LEVEL
            MiniMap = false; GoldMultiplier = 0; PlayerLeaving = false;
        }
        else { if (T()) return; }
        while (!PlayerWon) {
            NSF(); if (T()) return;
            while (!PlayerLeaving && !PlayerWon) {
                NSF(); if (T()) return;
                if (ArenaRoundWave == 0) {
                    if (T()) return;
                    ++AutoSaves; WhisperNeededForCutscene = true;
                    Pause p(*this);
                    START
                    std::string cs = "CS_ARENA_ROUND_BEGIN_WHISPER_GENERIC", theme = "NULL", say, title;
                    int choice = Rand() % 4;
                    if (choice == 1) { if (T()) return; cs = "CS_ARENA_ROUND_BEGIN_WHISPER_GENERIC2"; }
                    else if (choice == 2) { if (T()) return; cs = "CS_ARENA_ROUND_BEGIN_WHISPER_GENERIC3"; }
                    if (ArenaRound > 2) { if (T()) return; WhisperBound = true; if (!WhisperAllyAlive) { if (T()) return; WhisperAllyAlive = true; L("+ally"); } }
                    char o[32]; std::sprintf(o, "obj04_%02d", ArenaRound + 1);
                    static const int crowd[9] = { 0, 0, 1, 1, 2, 2, 3, 3, 3 };
                    static const char* const themes[9] = { 0, 0, "ENVIRONMENT_WITCHWOOD", "ENVIRONMENT_HAUNTED", "RESET", 0, 0, "ENVIRONMENT_EOW_04", "RESET" };
                    if (ArenaRound >= 0 && ArenaRound <= 8) {
                        L(o); NewCrowdBaseLevel = crowd[ArenaRound];
                        if (themes[ArenaRound]) theme = themes[ArenaRound];
                        switch (ArenaRound) {
                        case 0: {   // the announcement name of each hero title (the 20-entry table 0x00F14200; titles outside 1..20 get none)
                                    static const int nameOf[21] = { 0, 17, 20, 11, 6, 3, 12, 4, 15, 10, 13, 7, 16, 18, 9, 8, 19, 1, 14, 5, 2 };
                                    if (HeroTitle >= 1 && HeroTitle <= 20) { char t[32]; std::sprintf(t, "NAME_%02d", nameOf[HeroTitle]); title = t; }
                                }
                                cs = "CS_ARENA_ROUND_1"; Rand(); L("fanfare"); break;
                        case 1: cs = "CS_ARENA_ROUND_BEGIN_GENERIC"; say = "ROUND2"; break;
                        case 2: cs = "CS_ARENA_ROUND_WHISPER_INTRO"; break;
                        case 7: L("boss"); say = "ROUND7"; break;
                        case 8: ArenaState = MAS_IN_ARENA_WHISPER; cs = "CS_ARENA_ROUND_BOTH_WINNERS"; break;
                        default: { char s[32]; std::sprintf(s, "ROUND%d", ArenaRound); say = s; } break;
                        }
                    }
                    bool withWhisper = WhisperBound && WhisperAllyAlive;
                    if (withWhisper) { START }
                    L((cs + "[" + theme + "," + say + "," + title + (withWhisper ? ",W]" : "]")).c_str());
                    if (cs == "CS_ARENA_ROUND_BOTH_WINNERS") { if (T()) return; FinalBattleCS = true; }
                    WhisperNeededForCutscene = false;
                }
                if (!PlayerWon) {
                    if (T()) return;
                    if (ArenaState == MAS_IN_ARENA_WHISPER) {
                        if (T()) return;
                        WAIT_LEVEL
                        if (CellWhisperAlive) { if (T()) return; CellWhisperAlive = false; L("-cellwhisper"); }
                        PlayerWon = true;
                    }
                    else { if (T()) return; PlayWave(); }
                }
            }
            if (T()) return;
            if (PlayerLeaving) {
                if (T()) return;
                { Pause p(*this); L("leave"); }
                WAIT_LEVEL
                MiniMap = true; ArenaState = MAS_CELLS_MID_ARENA;
                CELL_WHISPER
                ++AutoSaves;
                while (ArenaState != MAS_IN_ARENA) { NSF(); if (T()) return; }
                if (T()) return;
                WAIT_LEVEL
                MiniMap = false; GoldMultiplier = 0; L("back");
            }
        }
        if (T()) return;
        {
            Pause p(*this);
            START START
            if (WhisperKilledByHero == true) { if (T()) return; L("CS_ARENA_CELLS_LADYGREY_TWO"); }
            else { if (T()) return; L("CS_ARENA_CELLS_LADYGREY_ONE"); }
        }
        WAIT_LEVEL
        L("CS_ARENA_HOH_OUTRO");
        ItemsReturned = true; MiniMap = true; MissionSucceeded = true;
    }
};

static Arena Make(int state, int terminateAt)
{
    Arena a; std::memset(static_cast<Fields*>(&a), 0, sizeof(Fields));
    a.ArenaState = state; a.TerminateAt = terminateAt; a.LevelDelay = 2; a.CellStay = 3; a.WavesPerRound = 2; a.LeaveAfterRound = -1;
    a.GoldMultiplier = 5; a.HeroTitle = 7;
    return a;
}
static int fails = 0;
static void check(bool c, const char* w) { if (!c) { ++fails; std::printf("FAIL %s\n", w); } }

static const int kRands[] = { 0, 9, 1, 2, 2, 1, 3, 5, 6, 0 };
static Arena Road(int terminateAt)
{
    Arena a = Make(MAS_ON_THE_WAY, terminateAt); a.HeroTitle = 0; a.StartFailures = 4; std::memcpy(a.Rands, kRands, sizeof kRands);
    return a;
}

int main()
{
    // 1. the whole quest from the road: hero title 0, four failed entity grabs, Whisper lines 0 / 1 / 2 / 3 (rounds 1-3 and 9 override)
    Arena a = Road(100000); a.Main();
    check(a.Log ==
        "obj01 holding chicken CS_ARENA_HOH_INTRO CS_ARENA_CELLS_INTRO obj02 obj03 CS_ARENA_CELLS_INTRO_CHAMDEAD gate "
        "obj04_01 fanfare CS_ARENA_ROUND_1[NULL,,NAME_17] wave wave "
        "obj04_02 CS_ARENA_ROUND_BEGIN_GENERIC[NULL,ROUND2,] wave wave "
        "obj04_03 CS_ARENA_ROUND_WHISPER_INTRO[ENVIRONMENT_WITCHWOOD,,] wave wave "
        "+ally obj04_04 CS_ARENA_ROUND_BEGIN_WHISPER_GENERIC3[ENVIRONMENT_HAUNTED,ROUND3,,W] wave wave "
        "obj04_05 CS_ARENA_ROUND_BEGIN_WHISPER_GENERIC2[RESET,ROUND4,,W] wave wave "
        "obj04_06 CS_ARENA_ROUND_BEGIN_WHISPER_GENERIC[NULL,ROUND5,,W] wave wave "
        "obj04_07 CS_ARENA_ROUND_BEGIN_WHISPER_GENERIC2[NULL,ROUND6,,W] wave wave "
        "obj04_08 boss CS_ARENA_ROUND_BEGIN_WHISPER_GENERIC3[ENVIRONMENT_EOW_04,ROUND7,,W] wave wave "
        "obj04_09 CS_ARENA_ROUND_BOTH_WINNERS[RESET,,,W] CS_ARENA_CELLS_LADYGREY_ONE CS_ARENA_HOH_OUTRO ", "full run log");
    check(a.Frame == 95 && a.AutoSaves == 9 && a.Paused == 0 && a.NewCrowdBaseLevel == 3 && a.Bindings == 14 && a.Watchers == 2, "full run counts");
    check(a.MissionSucceeded && a.FinalBattleCS && a.PlayerWon && a.ChamLeaving && a.HoldingActive && a.ItemsReturned && a.MiniMap
          && !a.WhisperNeededForCutscene && a.ArenaState == MAS_IN_ARENA_WHISPER && a.GoldMultiplier == 5 && a.HeroTitle == 1, "full run state");

    // 2. resumed in the cells at round 4 (holding quest active, title 7, Whisper killed by the hero), leaving once after round 5
    Arena b = Make(MAS_CELLS_MID_ARENA, 100000); b.ArenaRound = 3; b.HoldingActive = true; b.WhisperKilledByHero = true; b.LeaveAfterRound = 5;
    b.Rands[0] = 3; b.Rands[1] = 1; b.WhisperAllyAlive = true;
    b.Main();
    check(b.Log ==
        "obj01 +cellwhisper obj04_04 CS_ARENA_ROUND_BEGIN_WHISPER_GENERIC[ENVIRONMENT_HAUNTED,ROUND3,,W] wave wave "
        "obj04_05 CS_ARENA_ROUND_BEGIN_WHISPER_GENERIC2[RESET,ROUND4,,W] wave wave leave back "
        "obj04_06 CS_ARENA_ROUND_BEGIN_WHISPER_GENERIC[NULL,ROUND5,,W] wave wave "
        "obj04_07 CS_ARENA_ROUND_BEGIN_WHISPER_GENERIC[NULL,ROUND6,,W] wave wave "
        "obj04_08 boss CS_ARENA_ROUND_BEGIN_WHISPER_GENERIC[ENVIRONMENT_EOW_04,ROUND7,,W] wave wave "
        "obj04_09 CS_ARENA_ROUND_BOTH_WINNERS[RESET,,,W] -cellwhisper CS_ARENA_CELLS_LADYGREY_TWO CS_ARENA_HOH_OUTRO ", "resume log");
    check(b.MissionSucceeded && b.GoldMultiplier == 0 && b.AutoSaves == 7 && b.Paused == 0 && b.HeroTitle == 7 && !b.ChamLeaving, "resume state");

    // 3. the cell Whisper is removed before round 4; an ally alive in the world is not used before the script binds it; title 21 has no name
    Arena c = Make(MAS_CELLS_PRE_ARENA_NORMAL_GO, 100000); c.CellWhisperAlive = true; c.WhisperAllyAlive = true; c.HeroTitle = 21; c.HoldingActive = true;
    c.TerminateAt = 12; c.Main();
    check(c.Log == "obj01 -cellwhisper gate obj04_01 fanfare CS_ARENA_ROUND_1[NULL,,] wave wave obj04_02 CS_ARENA_ROUND_BEGIN_GENERIC[NULL,ROUND2,] wave wave obj04_03 CS_ARENA_ROUND_WHISPER_INTRO[ENVIRONMENT_WITCHWOOD,,] wave ", "pre-arena log");
    check(c.Frame == 12 && !c.MissionSucceeded && !c.PlayerWon && c.Paused == 0 && c.GoldMultiplier == 5 && !c.CellWhisperAlive, "pre-arena state");

    // 3b. the hero-title announcement: title 17 -> NAME_01, 20 -> NAME_02, 7 -> NAME_04, 1 -> NAME_17, 2 -> NAME_20, 21 -> none
    static const int titles[][2] = { { 17, 1 }, { 20, 2 }, { 7, 4 }, { 1, 17 }, { 2, 20 }, { 12, 16 } };
    for (int i = 0; i < 6; ++i) {
        Arena e = Make(MAS_CELLS_PRE_ARENA_NORMAL_GO, 8); e.HeroTitle = titles[i][0]; e.HoldingActive = true; e.Main();
        char want[64]; std::sprintf(want, "CS_ARENA_ROUND_1[NULL,,NAME_%02d] ", titles[i][1]);
        check(e.Log.find(want) != std::string::npos, "hero-title announcement");
    }
    // 4. termination at every frame of the full run: the script stops on that frame, nothing after it happens, the pause object is released
    for (int t = 0; t < 95; ++t) {
        Arena d = Road(t); d.Main();
        bool prefix = a.Log.compare(0, d.Log.size(), d.Log) == 0 && d.Log.size() < a.Log.size();
        if (!(d.Frame == t && d.Paused == 0 && !d.MissionSucceeded && !d.ItemsReturned && prefix)) { check(false, "termination sweep"); std::printf("  t=%d frame=%d\n", t, d.Frame); break; }
        if (t == 0) check(d.Log == "obj01 " && d.Bindings == 14 && d.Watchers == 0 && !d.HoldingActive && d.HeroTitle == 0, "terminated at start");
    }
    std::printf(fails == 0 ? "00f0fb70_TEST PASS\n" : "00f0fb70_TEST FAIL\n");
    return fails;
}
