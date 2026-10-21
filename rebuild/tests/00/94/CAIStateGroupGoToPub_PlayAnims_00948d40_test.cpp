// Behaviour test for CAIStateGroup_GoToPub::PlayAnims (0x00948D40): model copy with scripted stubs.
#include <cstdio>

struct CCharString { int id; };
struct CTCTavernGame { bool IsBeingPlayed; };
struct CThing { CTCTavernGame* game; bool GetInterface(CTCTavernGame** o, long t) { if (t != 0xcf || !game) return false; *o = game; return true; } };
struct CPointer { CThing* item; CThing* GetPItem() const { return item; } };
struct CThingCreatureBase { int id; };
struct CSoundPair { long FirstSound, LastSound; CSoundPair() : FirstSound(0), LastSound(0) { } };
struct CCreatureActionBase { int anim; bool usePhysics; };
struct CCreatureAction_PlayAnimation : CCreatureActionBase {
    CCreatureAction_PlayAnimation(CThingCreatureBase*, const CCharString& n, const CSoundPair&, bool, bool p, bool, long, long) { anim = n.id; usePhysics = p; }
};
struct CAIBrain { CThingCreatureBase* PCreature; CThingCreatureBase* GetPCreature() const { return PCreature; } };

static int g_seq[32], g_n, g_results[32], g_waitless, g_stopAt;
static CTCTavernGame* g_game;

struct CAIStateGroup_GoToPub {
    CAIBrain* PBrain; CPointer PTable;
    long Run(CCreatureActionBase* a) { if (g_n == g_stopAt && g_game) g_game->IsBeingPlayed = true; g_seq[g_n] = a->anim; return g_results[g_n++]; }
    long AIAction_PerformAction(CCreatureActionBase* a) { ++g_waitless; return Run(a); }
    long AIAction_PerformActionAndWait(CCreatureActionBase* a, bool wait) { return wait ? -1 : Run(a); }
    bool PlayAnims(const CCharString& intoAnim, const CCharString& loopAnim, const CCharString& outAnim, long loopCount);
};

bool CAIStateGroup_GoToPub::PlayAnims(const CCharString& intoAnim, const CCharString& loopAnim, const CCharString& outAnim, long loopCount)
{
    CTCTavernGame* game = 0;
    PTable.GetPItem()->GetInterface(&game, 0xcf);
    CCreatureAction_PlayAnimation into(PBrain->GetPCreature(), intoAnim, CSoundPair(), false, true, false, 0, 0);
    bool failed = AIAction_PerformActionAndWait(&into, false) != 2;
    if (failed)
        return false;
    while (loopCount > 0) {
        --loopCount;
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

static void Reset() { g_n = g_waitless = 0; g_stopAt = -1; for (int i = 0; i < 32; ++i) { g_results[i] = 2; g_seq[i] = 0; } }

int main()
{
    int ok = 1;
    CCharString a = { 1 }, b = { 2 }, c = { 3 };
    CTCTavernGame game = { false };
    CThing table = { &game };
    CThingCreatureBase me = { 0 }; CAIBrain brain = { &me };
    CAIStateGroup_GoToPub s; s.PBrain = &brain; s.PTable.item = &table; g_game = &game;
    // into, three loops, out
    Reset(); if (!s.PlayAnims(a, b, c, 3) || g_n != 5 || g_seq[0] != 1 || g_seq[3] != 2 || g_seq[4] != 3 || g_waitless != 1) ok = 0;
    // zero loops
    Reset(); if (!s.PlayAnims(a, b, c, 0) || g_n != 2 || g_seq[1] != 3) ok = 0;
    // the out-of animation fails
    Reset(); g_results[2] = 1; if (s.PlayAnims(a, b, c, 1) || g_n != 3) ok = 0;
    // into fails
    Reset(); g_results[0] = 0; if (s.PlayAnims(a, b, c, 4) || g_n != 1) ok = 0;
    // a loop fails
    Reset(); g_results[2] = 3; if (s.PlayAnims(a, b, c, 4) || g_n != 3) ok = 0;
    // the game starts during the second play: stop early with true
    Reset(); g_stopAt = 1; if (!s.PlayAnims(a, b, c, 4) || g_n != 2) ok = 0;
    game.IsBeingPlayed = false;
    // no game component: plays through
    table.game = 0; g_game = 0;
    Reset(); if (!s.PlayAnims(a, b, c, 2) || g_n != 4) ok = 0;
    if (!ok) { std::printf("00948d40_TEST FAIL\n"); return 1; }
    std::printf("00948d40_TEST PASS\n");
    return 0;
}