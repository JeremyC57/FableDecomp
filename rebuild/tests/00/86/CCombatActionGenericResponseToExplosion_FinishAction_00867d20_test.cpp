// Behaviour test for CCombatAction_GenericResponseToExplosion::FinishAction (0x00867D20): the base FinishAction runs; the flip is kept only
// when FlipFacing is set, the animation (if any) has no rotation and the current mode is not 29; mode 29 is removed; with health below
// DELTA_NOTIONAL_ZERO the creature dies (def leaves a dead creature) or is killed, else it is stunned (knockdown-to-stun) or gets up;
// mode 66 is removed last on every path.
#include <cstdio>
#include <vector>
struct World { int base_calls; long anim; bool rotation; long current_mode; float health; bool leave_dead; bool to_stun; int killed;
               int next_action; bool next_flip; std::vector<unsigned long> removed; };
static void FinishAction(World& w, bool FlipFacing)
{
    ++w.base_calls;
    long anim_index = w.anim;
    bool flip = FlipFacing;
    if (flip) {
        if (anim_index) {
            if (w.rotation) flip = false;
        }
        if (flip && w.current_mode == 0x1d) flip = false;
    }
    w.removed.push_back(0x1d);
    if (w.health < 0.0001f) {
        if (w.leave_dead) { w.next_action = 1; w.next_flip = flip; }
        else ++w.killed;
    } else if (w.to_stun) { w.next_action = 2; w.next_flip = flip; }
    else { w.next_action = 3; w.next_flip = flip; }
    w.removed.push_back(0x42);
}
static World make() { World w; w.base_calls = 0; w.anim = 5; w.rotation = false; w.current_mode = 3; w.health = 10.0f; w.leave_dead = true;
    w.to_stun = false; w.killed = 0; w.next_action = 0; w.next_flip = false; return w; }
static int fail(int n) { std::printf("00867d20_TEST FAIL %d\n", n); return 1; }
int main()
{
    World w = make(); FinishAction(w, true);
    if (w.base_calls != 1 || w.next_action != 3 || !w.next_flip || w.removed.size() != 2 || w.removed[0] != 0x1d || w.removed[1] != 0x42) return fail(1);
    w = make(); w.rotation = true; FinishAction(w, true);
    if (w.next_flip) return fail(2);                                     // rotating animation vetoes the flip
    w = make(); w.anim = 0; w.rotation = true; FinishAction(w, true);
    if (!w.next_flip) return fail(3);                                    // no animation: stats not consulted
    w = make(); w.current_mode = 0x1d; FinishAction(w, true);
    if (w.next_flip) return fail(4);                                     // current mode 29 vetoes
    w = make(); FinishAction(w, false);
    if (w.next_flip) return fail(5);
    w = make(); w.health = 0.0f; FinishAction(w, true);
    if (w.next_action != 1 || !w.next_flip || w.killed || w.removed.size() != 2) return fail(6);   // dies
    w = make(); w.health = 0.0f; w.leave_dead = false; FinishAction(w, true);
    if (w.next_action != 0 || w.killed != 1 || w.removed.back() != 0x42) return fail(7);           // killed outright
    w = make(); w.to_stun = true; FinishAction(w, true);
    if (w.next_action != 2) return fail(8);                                                         // stunned
    w = make(); w.health = 0.0001f; FinishAction(w, true);                                           // health == DELTA_NOTIONAL_ZERO (1e-4f, 0x0129BA3C): not below, gets up
    if (w.next_action != 3 || w.killed) return fail(9);
    w = make(); w.to_stun = true; w.rotation = true; FinishAction(w, true);                           // the stunned arm gets the vetoed flip
    if (w.next_action != 2 || w.next_flip) return fail(10);
    w = make(); w.health = 0.0f; w.current_mode = 0x1d; FinishAction(w, true);                       // the die arm gets the vetoed flip
    if (w.next_action != 1 || w.next_flip) return fail(11);
    std::printf("00867d20_TEST PASS\n");
    return 0;
}