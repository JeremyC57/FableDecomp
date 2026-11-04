// Behaviour test for 0x0070D780 CAnimateThing::AddCombinationAnim(const CAnimInit&, long, long, bool, bool): a model; checks the blocked / no-anim early outs, the fade-in (internal alpha 0, change 1/frames) or fully-on set, and the list chosen by the delay.
#include <cstdio>
#include <cmath>
#include <vector>
#include <string>
#include <set>
#include <map>

struct Set { bool owned; float lastAlpha, currentAlpha, internalCurrent, internalLast, change; long delay; };
struct Animator { unsigned long MainFlags; std::vector<Set> combos, delayed; };
static void Add(Animator& a, long anim, float initLast, long fade, long delay, bool force, bool owned)
{
    if (!force && (a.MainFlags & 1)) return;
    if (!anim) return;
    Set s = { owned, initLast, 0.5f, 0.5f, 0.5f, 0.0f, delay };
    if (fade > 0) { s.internalLast = s.internalCurrent; s.internalCurrent = 0.0f; s.internalLast = 0.0f; s.change = 1.0f / fade; }
    else { s.currentAlpha = 1.0f; s.lastAlpha = 1.0f; s.change = 0.0f; }
    (delay == 0 ? a.combos : a.delayed).push_back(s);
}
int main()
{
    int ok = 1; Animator a; a.MainFlags = 1;
    Add(a, 3, 0.2f, 4, 0, false, true); ok &= a.combos.empty();
    Add(a, 3, 0.2f, 4, 0, true, true); ok &= a.combos.size() == 1 && a.combos[0].internalCurrent == 0.0f && a.combos[0].change == 0.25f && a.combos[0].lastAlpha == 0.2f;
    a.MainFlags = 0; Add(a, 0, 0, 0, 0, false, false); ok &= a.combos.size() == 1;
    Add(a, 3, 0.2f, 0, 5, false, false); ok &= a.delayed.size() == 1 && a.delayed[0].currentAlpha == 1.0f && a.delayed[0].delay == 5;
    std::printf(ok ? "0070D780_TEST PASS\n" : "0070D780_TEST FAIL\n"); return ok ? 0 : 1;
}
