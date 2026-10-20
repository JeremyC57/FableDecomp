// Behaviour test for CTCCreatureModeManager::SetupInitialMode (0x0073AFC0): weapon mode when carrying, idle swap when
// the front mode allows it and idle modes exist, then the initial mode; defs released.
#include <cstdio>

static long g_added[8]; static int g_nadded = 0; static long g_removed = -1;
struct ModeDef { long refs; long Initial; long Weapon; long idle[3]; int nidle; };
struct Manager {
    bool hasDef, found, carrying, frontIdle; long frontType;
    ModeDef* sub; long thingDefRefs;
    void AddMode(long m) { g_added[g_nadded++] = m; }
    void RemoveMode(long m) { g_removed = m; }
    unsigned long Random(unsigned long range) { return range - 1; }
    void SetupInitialMode()
    {
        if (!hasDef) return;
        ++thingDefRefs;
        ModeDef* def = found ? sub : 0;
        if (def) ++def->refs;
        if (found) {
            if (carrying) AddMode(def->Weapon);
            if (frontIdle && def->nidle != 0) {
                RemoveMode(frontType);
                unsigned long count = def->nidle;
                AddMode(def->idle[Random(count)]);
            }
            if (def->Initial) AddMode(def->Initial);
        }
        if (def) --def->refs;
        --thingDefRefs;
    }
};

int main()
{
    ModeDef d = { 1, 5, 9, { 11, 12, 13 }, 3 };
    Manager m = { true, true, true, true, 4, &d, 1 };
    m.SetupInitialMode();
    if (g_nadded != 3 || g_added[0] != 9 || g_added[1] != 13 || g_added[2] != 5 || g_removed != 4) {
        std::printf("0073afc0_TEST FAIL\n"); return 1;
    }
    if (d.refs != 1 || m.thingDefRefs != 1) { std::printf("0073afc0_TEST FAIL\n"); return 1; }
    g_nadded = 0; g_removed = -1; m.carrying = false; d.nidle = 0; d.Initial = 0;
    m.SetupInitialMode();
    if (g_nadded != 0 || g_removed != -1) { std::printf("0073afc0_TEST FAIL\n"); return 1; }
    m.found = false; d.Initial = 5; m.SetupInitialMode();
    if (g_nadded != 0) { std::printf("0073afc0_TEST FAIL\n"); return 1; }
    std::printf("0073afc0_TEST PASS\n");
    return 0;
}