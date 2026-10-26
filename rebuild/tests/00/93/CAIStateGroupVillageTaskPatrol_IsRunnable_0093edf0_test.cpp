#include <cstdio>
// Model of CAIStateGroup_VillageTaskPatrol::IsRunnable @ 0x0093EDF0.
class CThing { public: int id; };
class CTCVillage { public: long has; long HasPatroller(CThing* c) { return c->id == 1 ? has : 0; } };
static CTCVillage g_village; static bool g_villageOk; static CThing* g_item; static int g_slot10;
struct Group {
    CThing creature;
    CThing* GetPItem() { return g_item; }
    void OnTaskThingFound() { ++g_slot10; }
    bool IsRunnable()
    {
        CTCVillage* village = g_villageOk ? &g_village : 0;
        if (village != 0) {
            if (GetPItem() != 0) OnTaskThingFound();
            CThing* c = &creature;
            return village->HasPatroller(c) != 0;
        }
        return false;
    }
};
int main()
{
    Group g; g.creature.id = 1; g_villageOk = false; g_item = 0; g_slot10 = 0; g_village.has = 5;
    if (g.IsRunnable() || g_slot10 != 0) { std::printf("0093edf0_TEST FAIL\n"); return 1; }
    g_villageOk = true;
    if (!g.IsRunnable() || g_slot10 != 0) { std::printf("0093edf0_TEST FAIL\n"); return 1; }
    g_item = &g.creature; g_village.has = 0;
    if (g.IsRunnable() || g_slot10 != 1) { std::printf("0093edf0_TEST FAIL\n"); return 1; }
    std::printf("0093edf0_TEST PASS\n"); return 0;
}