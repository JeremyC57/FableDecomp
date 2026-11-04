// Behaviour test for 0x0070D780 CAnimateThing::AddCombinationAnim: a model. Nothing is added when the main set blocks combinations
// (Flags bit 0) and the add is not forced, or when the init has no anim. Otherwise a set built from the init gets the owned-by-main
// flag, the init's LastAlpha and the delay; with fade-in frames its internal alpha 0 restarts at 0 and the alpha change is
// 1/frames, else the set is fully on with no change; it goes to the combination list (delay 0) or the delayed list.
#include <cstdio>
#include <list>

struct Init { long anim; float alpha, lastAlpha; };
struct Set { long anim; float currentAlpha, lastAlpha, changeAlphaBy, internalCurrent, internalLast; long delay; bool owned; };
struct Animate { unsigned long mainFlags; std::list<Set> combination, delayed; };

static void AddCombinationAnim(Animate& a, const Init& init, long fadeInFrames, long delay, bool force, bool owned)
{
    if (!force && (a.mainFlags & 1))
        return;
    if (!init.anim)
        return;
    Set s = { init.anim, init.alpha, init.alpha, 0.0f, init.alpha, init.alpha, 0, false };   // Init
    s.lastAlpha = init.lastAlpha;
    s.owned = owned;
    s.delay = delay;
    if (fadeInFrames > 0) {
        s.internalLast = s.internalCurrent;
        s.internalCurrent = 0.0f;
        s.internalLast = 0.0f;
        s.changeAlphaBy = 1.0f / fadeInFrames;
    } else {
        s.currentAlpha = 1.0f;
        s.lastAlpha = 1.0f;
        s.changeAlphaBy = 0.0f;
    }
    (delay == 0 ? a.combination : a.delayed).push_back(s);
}

int main()
{
    int ok = 1;
    Animate a;
    a.mainFlags = 1;
    Init i = { 7, 0.5f, 0.25f };
    AddCombinationAnim(a, i, 0, 0, false, false);
    ok &= a.combination.empty() && a.delayed.empty();
    AddCombinationAnim(a, i, 4, 0, true, true);
    ok &= a.combination.size() == 1;
    const Set& s = a.combination.back();
    ok &= s.owned && s.lastAlpha == 0.25f && s.currentAlpha == 0.5f && s.internalCurrent == 0.0f && s.changeAlphaBy == 0.25f;
    a.mainFlags = 0;
    AddCombinationAnim(a, i, 0, 3, false, false);
    ok &= a.delayed.size() == 1 && a.delayed.back().currentAlpha == 1.0f && a.delayed.back().lastAlpha == 1.0f && a.delayed.back().delay == 3;
    Init none = { 0, 1, 1 };
    AddCombinationAnim(a, none, 0, 0, true, false);
    ok &= a.combination.size() == 1 && a.delayed.size() == 1;
    std::printf(ok ? "0070D780_TEST PASS\n" : "0070D780_TEST FAIL\n");
    return ok ? 0 : 1;
}