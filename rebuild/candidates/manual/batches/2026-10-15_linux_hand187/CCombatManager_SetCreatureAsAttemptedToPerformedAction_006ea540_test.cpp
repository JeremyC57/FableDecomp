#include <cstdio>
// Behaviour test for CCombatManager::SetCreatureAsAttemptedToPerformedAction @ 0x006EA540 (decision logic).
struct Seq {
    bool m_finished; bool canContinue; int finishCalls; int performedCalls;
    void OnFinish() { ++finishCalls; }
    void Finish() { if (!m_finished) { m_finished = true; OnFinish(); } }
    void OnActionPerformed() { ++performedCalls; }
};
struct Info { long CurrentSequence; Seq* seqs[2]; };
static int g_resets = 0;
static void Attempt(bool eligible, Info& info, bool performed)
{
    if (!eligible) return;
    if (info.CurrentSequence < 0) return;
    Seq* sequence = info.seqs[info.CurrentSequence];
    if (performed) { sequence->Finish(); sequence->OnActionPerformed(); }
    else if (!sequence->canContinue) sequence->Finish();
    else if (!sequence->m_finished) ++g_resets;
}
static int Fail() { std::printf("006ea540_TEST FAIL\n"); return 1; }
int main()
{
    Seq a = {false, false, 0, 0}; Seq b = {false, true, 0, 0};
    Info info; info.seqs[0] = &a; info.seqs[1] = &b;
    info.CurrentSequence = 0; Attempt(true, info, true); Attempt(true, info, true);
    if (a.finishCalls != 1 || a.performedCalls != 2) return Fail();
    info.CurrentSequence = 1; Attempt(true, info, false);
    if (g_resets != 1 || b.finishCalls != 0) return Fail();
    b.canContinue = false; Attempt(true, info, false);
    if (b.finishCalls != 1 || !b.m_finished) return Fail();
    b.canContinue = true; Attempt(true, info, false);
    if (g_resets != 1) return Fail();
    info.CurrentSequence = -1; Attempt(true, info, true); Attempt(false, info, true);
    if (a.performedCalls != 2) return Fail();
    std::printf("006ea540_TEST PASS\n");
    return 0;
}
