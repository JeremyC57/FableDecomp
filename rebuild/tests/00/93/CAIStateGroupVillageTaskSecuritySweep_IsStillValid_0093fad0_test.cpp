#include <cstdio>
// Model of CAIStateGroup_VillageTaskSecuritySweep::IsStillValid @ 0x0093FAD0.
struct CThing { int id; };
struct Task { bool enabled, active, valid; bool IsEnabled() { return enabled; } bool IsActive() { return active; } bool IsValidFor(CThing* c) { return valid && c->id == 9; } };
static Task* g_task;
struct Group {
    CThing creature;
    Task* GetPItem() const { return g_task; }
    bool IsStillValid() const
    {
        if (GetPItem() != 0 && GetPItem()->IsEnabled()) {
            if (GetPItem()->IsActive() || GetPItem()->IsValidFor(const_cast<CThing*>(&creature))) return true;
        }
        return false;
    }
};
int main()
{
    Group g; g.creature.id = 9; Task t = { true, false, true }; g_task = 0;
    if (g.IsStillValid()) { std::printf("0093fad0_TEST FAIL\n"); return 1; }
    g_task = &t;
    if (!g.IsStillValid()) { std::printf("0093fad0_TEST FAIL\n"); return 1; }
    t.valid = false; if (g.IsStillValid()) { std::printf("0093fad0_TEST FAIL\n"); return 1; }
    t.active = true; if (!g.IsStillValid()) { std::printf("0093fad0_TEST FAIL\n"); return 1; }
    t.enabled = false; if (g.IsStillValid()) { std::printf("0093fad0_TEST FAIL\n"); return 1; }
    std::printf("0093fad0_TEST PASS\n"); return 0;
}