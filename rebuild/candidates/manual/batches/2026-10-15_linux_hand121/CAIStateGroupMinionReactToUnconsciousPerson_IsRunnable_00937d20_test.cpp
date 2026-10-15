#include <cstdio>
// MinionReactToUnconsciousPerson::IsRunnable: pick the finder by life state, accept a living person, remember it
struct Thing { unsigned char flags91; };
struct Ptr { Thing* item; void SetPItem(Thing* p) { item = p; } Thing* GetPItem() const { return item; } };
static Thing* g_undiscovered = 0; static Thing* g_any = 0; static int g_calls[2];
struct Group {
    long lifeState; Ptr person;
    Thing* FindUndiscovered() { ++g_calls[0]; return g_undiscovered; }
    Thing* FindAny() { ++g_calls[1]; return g_any; }
    bool IsRunnable()
    {
        Thing* p = (lifeState == 3) ? FindAny() : FindUndiscovered();
        if (p != 0 && !(p->flags91 & 1)) { Ptr temp; temp.SetPItem(p); person.SetPItem(temp.GetPItem()); return true; }
        return false;
    }
};
int main()
{
    Thing a; a.flags91 = 0; Thing b; b.flags91 = 0; g_undiscovered = &a; g_any = &b; g_calls[0] = g_calls[1] = 0;
    Group g; g.lifeState = 0; g.person.item = 0;
    if (!g.IsRunnable() || g.person.item != &a || g_calls[0] != 1 || g_calls[1] != 0) { std::printf("00937d20_TEST FAIL\n"); return 1; }
    g.lifeState = 3; if (!g.IsRunnable() || g.person.item != &b || g_calls[1] != 1) { std::printf("00937d20_TEST FAIL\n"); return 1; }
    b.flags91 = 1; if (g.IsRunnable() || g.person.item != &b) { std::printf("00937d20_TEST FAIL\n"); return 1; }
    g_any = 0; if (g.IsRunnable()) { std::printf("00937d20_TEST FAIL\n"); return 1; }
    std::printf("00937d20_TEST PASS\n"); return 0;
}
