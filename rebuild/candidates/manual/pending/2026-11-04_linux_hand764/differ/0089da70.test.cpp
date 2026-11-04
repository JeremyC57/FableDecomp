// Behaviour test for 0x0089DA70 CGameScriptInterface::EntitySetAlpha(const CScriptThing&, float, bool) const: a model; checks the clamp to [0, 1] scaled to a byte, nothing for a dead entity or one without an appearance, and the carried things (live, with an appearance) only when asked.
#include <cstdio>
#include <cmath>
#include <vector>
#include <string>
#include <set>
#include <map>

struct Thing { bool alive, hasApp; int alpha; std::vector<Thing*> carried; };
static void SetAlpha(Thing& e, float a, bool carried)
{
    if (!e.alive || !e.hasApp) return;
    if (a < 0.0f) a = 0.0f; else if (a > 1.0f) a = 1.0f;
    e.alpha = (unsigned char)(long)(a * 255.0f);
    if (carried) for (size_t i = 0; i < e.carried.size(); ++i) { Thing* t = e.carried[i]; if (t && t->alive && t->hasApp) t->alpha = (unsigned char)(long)(a * 255.0f); }
}
int main()
{
    int ok = 1; Thing c1 = { true, true, 0 }, c2 = { false, true, 0 }, e = { true, true, 0 };
    e.carried.push_back(&c1); e.carried.push_back(&c2); e.carried.push_back(0);
    SetAlpha(e, 2.0f, false); ok &= e.alpha == 255 && c1.alpha == 0;
    SetAlpha(e, 0.5f, true); ok &= e.alpha == 127 && c1.alpha == 127 && c2.alpha == 0;
    SetAlpha(e, -1.0f, true); ok &= e.alpha == 0 && c1.alpha == 0;
    e.alive = false; SetAlpha(e, 1.0f, true); ok &= e.alpha == 0;
    std::printf(ok ? "0089DA70_TEST PASS\n" : "0089DA70_TEST FAIL\n"); return ok ? 0 : 1;
}
