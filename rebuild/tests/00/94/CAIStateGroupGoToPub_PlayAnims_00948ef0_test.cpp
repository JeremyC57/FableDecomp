// Behaviour test for CAIStateGroup_GoToPub::PlayAnims(..., float seconds) (0x00948EF0): model copy with scripted stubs.
#include <cstdio>

struct CCharString { int id; };
struct CTCTavernGame { bool IsBeingPlayed; };
struct CThing { CTCTavernGame* game; bool GetInterface(CTCTavernGame** o, long t) { if (t != 0xcf || !game) return false; *o = game; return true; } };
struct CPointer { CThing* item; CThing* GetPItem() const { return item; } };
struct CThingCreatureBase { int id; };
struct CSoundPair { long FirstSound, LastSound; CSoundPair() : FirstSound(0), LastSound(0) { } };
struct CCreatureActionBase { int anim; };
struct CCreatureAction_PlayAnimation : CCreatureActionBase {
    CCreatureAction_PlayAnimation(CThingCreatureBase*, const CCharString& n, const CSoundPair&, bool, bool, bool, long, long) { anim = n.id; }
};
struct CAIBrain { CThingCreatureBase* PCreature; CThingCreatureBase* GetPCreature() const { return PCreature; } };
static long g_framesPerSecond = 30;
static long g_frames[32]; static int g_fi;
struct CWorld { static long GetWorldFrame() { return g_frames[g_fi++]; } };

static int g_seq[32], g_n, g_results[32], g_waitless;

struct CAIStateGroup_GoToPub {
    CAIBrain* PBrain; CPointer PTable;
    long Run(CCreatureActionBase* a) { g_seq[g_n] = a->anim; return g_results[g_n++]; }
    long AIAction_PerformAction(CCreatureActionBase* a) { ++g_waitless; return Run(a); }
    long AIAction_PerformActionAndWait(CCreatureActionBase* a, bool wait) { return wait ? -1 : Run(a); }
    bool PlayAnims(const CCharString& intoAnim, const CCharString& loopAnim, const CCharString& outAnim, float seconds);
};

bool CAIStateGroup_GoToPub::PlayAnims(const CCharString& intoAnim, const CCharString& loopAnim, const CCharString& outAnim, float seconds)
{
    CTCTavernGame* game = 0;
    PTable.GetPItem()->GetInterface(&game, 0xcf);
    CCreatureAction_PlayAnimation into(PBrain->GetPCreature(), intoAnim, CSoundPair(), false, true, false, 0, 0);
    bool failed = AIAction_PerformActionAndWait(&into, false) != 2;
    if (failed)
        return false;
    long fps = g_framesPerSecond;
    float endFrame = (float)CWorld::GetWorldFrame() + fps * seconds;
    while ((float)CWorld::GetWorldFrame() > endFrame) {
        if (game && game->IsBeingPlayed)
            return true;
        CCreatureAction_PlayAnimation loop(PBrain->GetPCreature(), loopAnim, CSoundPair(), false, true, false, 0, 0);
        failed = AIAction_PerformActionAndWait(&loop, false) != 2;
        if (failed)
            return false;
    }
    if (game && game->IsBeingPlayed)
        return true;
    CCreatureAction_PlayAnimation out(PBrain->GetPCreature(), outAnim, CSoundPair(), false, true, false, 0, 0);
    failed = AIAction_PerformAction(&out) != 2;
    if (failed)
        return false;
    return true;
}

static void Reset() { g_n = g_waitless = g_fi = 0; for (int i = 0; i < 32; ++i) { g_results[i] = 2; g_seq[i] = 0; g_frames[i] = 100; } }

int main()
{
    int ok = 1;
    CCharString a = { 1 }, b = { 2 }, c = { 3 };
    CTCTavernGame game = { false };
    CThing table = { &game };
    CThingCreatureBase me = { 0 }; CAIBrain brain = { &me };
    CAIStateGroup_GoToPub s; s.PBrain = &brain; s.PTable.item = &table;
    // positive duration: the frame is never past the end, so into then out (retail behaviour)
    Reset(); if (!s.PlayAnims(a, b, c, 2.0f) || g_n != 2 || g_seq[1] != 3 || g_waitless != 1) ok = 0;
    // negative duration (end 100 - 60 = 40): frames 100, 100 loop twice, then 30 ends the loop
    Reset(); g_frames[3] = 30; if (!s.PlayAnims(a, b, c, -2.0f) || g_n != 4 || g_seq[1] != 2 || g_seq[2] != 2 || g_seq[3] != 3) ok = 0;
    // a loop play fails
    Reset(); g_results[1] = 0; if (s.PlayAnims(a, b, c, -2.0f) || g_n != 2) ok = 0;
    // into fails
    Reset(); g_results[0] = 1; if (s.PlayAnims(a, b, c, 1.0f) || g_n != 1) ok = 0;
    // out fails
    Reset(); g_results[1] = 4; if (s.PlayAnims(a, b, c, 1.0f) || g_n != 2) ok = 0;
    // the game is being played: true before the out anim
    Reset(); game.IsBeingPlayed = true; if (!s.PlayAnims(a, b, c, 1.0f) || g_n != 1) ok = 0;
    Reset(); if (!s.PlayAnims(a, b, c, -1.0f) || g_n != 1) ok = 0;
    if (!ok) { std::printf("00948ef0_TEST FAIL\n"); return 1; }
    std::printf("00948ef0_TEST PASS\n");
    return 0;
}