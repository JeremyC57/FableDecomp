// Behaviour test for 006a22d0: CTCCarrying::RemoveAllThingsInPrimarySlot -- every primary slot's thing is removed and appended (model copy, stand-in defs).
#include <cstdio>
#include <vector>
class CThing { public: int id; };
struct SEntry { long first; CThing* second; };
static bool g_primary[8];
static bool g_lastFlag = false;
static int g_released = 0;
class CTCCarrying { public:
    SEntry* first; SEntry* last;
    bool IsSlotPrimarySlot(long slot) { ++g_released; return g_primary[slot]; }   // def fetched and released once
    CThing* RemoveThingInCarrySlot(long slot, bool flag)
    {
        g_lastFlag = flag;
        for (SEntry* e = first; e != last; ++e) if (e->first == slot) { CThing* t = e->second; e->second = 0; return t; }
        return 0;
    }
    void RemoveAllThingsInPrimarySlot(std::vector<CThing*>& removed, bool flag)
    {
        for (SEntry* it = first; it != last; ++it)
            if (IsSlotPrimarySlot(it->first)) removed.push_back(RemoveThingInCarrySlot(it->first, flag));
    }
};
int main()
{
    CThing a = {1}, b = {2}, c = {3};
    SEntry slots[4] = { {0, &a}, {2, &b}, {5, 0}, {6, &c} };
    g_primary[2] = true; g_primary[5] = true; g_primary[6] = true;
    CTCCarrying carrying = { slots, slots + 4 };
    std::vector<CThing*> removed;
    carrying.RemoveAllThingsInPrimarySlot(removed, true);
    int fail = 0;
    if (removed.size() != 3 || removed[0] != &b || removed[1] != 0 || removed[2] != &c) fail = 1;   // an empty primary slot appends null
    if (slots[0].second != &a || slots[1].second != 0 || slots[3].second != 0) fail = 1;
    if (!g_lastFlag || g_released != 4) fail = 1;
    if (fail) { std::printf("006a22d0_TEST FAIL\n"); return 1; }
    std::printf("006a22d0_TEST PASS\n");
    return 0;
}