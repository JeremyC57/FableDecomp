// Behaviour test for CCreatureAction_UnsheatheItemFromInventoryWithHasteAction::FinishAction (0x006A02B0), g++ model of the parked source.
#include <cstdio>
#include <map>
struct Haste { std::map<void*, float> multipliers; };
struct Creature { bool bulletTime; float timeMultiplier; Haste* haste; Haste added; int addCalls; };
static int g_baseFinish;
static void FinishAction(Creature& c, void* key)
{
    ++g_baseFinish;
    if (c.bulletTime) {
        Haste* h = c.haste;
        if (!h) { ++c.addCalls; h = c.haste = &c.added; }
        h->multipliers[key] = c.timeMultiplier;
    }
}
int main()
{
    int key;
    Creature c = { true, 0.5f, 0, Haste(), 0 };
    FinishAction(c, &key);
    int ok = g_baseFinish == 1 && c.addCalls == 1 && c.haste == &c.added && c.added.multipliers[&key] == 0.5f;
    c.timeMultiplier = 0.25f; FinishAction(c, &key); ok &= c.addCalls == 1 && c.added.multipliers[&key] == 0.25f;
    Creature d = { false, 0.5f, 0, Haste(), 0 }; FinishAction(d, &key); ok &= d.addCalls == 0 && d.haste == 0 && g_baseFinish == 3;
    std::printf(ok ? "006a02b0_TEST PASS\n" : "006a02b0_TEST FAIL\n");
    return ok ? 0 : 1;
}
