// Behaviour test for CAIStateGroup_CreatureTakeCover::Update (0x0097A630): target/cover checks, low cover duck-and-attack, high cover step-out cycle, list refresh.
#include <cstdio>
#include <cstring>
struct TakeCover {
    bool targetAlive, coverAlive, haveCoverPos, lowCover, canSee, moveIn, runnable, attackOk;
    float distance; int failedCount, listSize; unsigned int rnd;
    int terminated, failedMarks, crouches, moves, ducks, attacks, reloads, removed, refreshed, cleared, stepOuts;
    void Terminate() { ++terminated; }
    bool MoveIntoCover() { ++moves; return moveIn; }
    bool Attack() { ++attacks; return attackOk; }
    void Update()
    {
        if (!targetAlive) { Terminate(); return; }
        bool coverWasAlive = coverAlive;
        if (!haveCoverPos) { if (failedCount <= 0) ++failedMarks; Terminate(); return; }
        bool far = distance > 1.8f;
        if (far) {
            if (coverWasAlive) { if (!runnable) { Terminate(); return; } ++crouches; }
            if (!MoveIntoCover()) { ++removed; ++cleared; goto refresh; }
        }
        if (lowCover) {
            unsigned int n = rnd % 3 + 1;
            for (unsigned int i = 0; i < n; ++i) {
                ++ducks;
                if (!runnable || !Attack()) { Terminate(); return; }
            }
            ++ducks;
        } else {
            unsigned int n = rnd % 3 + 1;
            if (!targetAlive || !coverAlive) { Terminate(); return; }
            for (unsigned int i = 0; i < n; ++i) {
                if (!runnable) { Terminate(); return; }
                bool fire = canSee || (++stepOuts, true);
                if (fire) {
                    if (!Attack()) { Terminate(); return; }
                    if (MoveIntoCover()) ++reloads;
                }
            }
        }
        ++removed;
    refresh:
        if (failedCount > 0 && listSize == 0) ++refreshed;
    }
};
static TakeCover base()
{
    TakeCover t; std::memset(&t, 0, sizeof(t));
    t.targetAlive = t.coverAlive = t.haveCoverPos = t.moveIn = t.runnable = t.attackOk = true; t.canSee = true; return t;
}
int main()
{
    TakeCover a = base(); a.lowCover = true; a.rnd = 5; a.Update();
    if (a.attacks != 3 || a.ducks != 4 || a.removed != 1 || a.terminated) { std::printf("0097a630_TEST FAIL\n"); return 1; }
    TakeCover b = base(); b.rnd = 1; b.distance = 5.0f; b.Update();
    if (b.crouches != 1 || b.attacks != 2 || b.reloads != 2 || b.moves != 3 || b.removed != 1) { std::printf("0097a630_TEST FAIL\n"); return 1; }
    TakeCover c = base(); c.distance = 5.0f; c.moveIn = false; c.failedCount = 1; c.Update();
    if (c.cleared != 1 || c.refreshed != 1 || c.attacks != 0) { std::printf("0097a630_TEST FAIL\n"); return 1; }
    TakeCover d = base(); d.haveCoverPos = false; d.Update();
    if (d.failedMarks != 1 || d.terminated != 1) { std::printf("0097a630_TEST FAIL\n"); return 1; }
    TakeCover e = base(); e.targetAlive = false; e.Update();
    if (e.terminated != 1 || e.moves != 0) { std::printf("0097a630_TEST FAIL\n"); return 1; }
    std::printf("0097a630_TEST PASS\n");
    return 0;
}
