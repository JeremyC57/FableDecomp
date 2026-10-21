// Behaviour test for CTCObjectAugmentations::OnSerialise (0x00767200): saving writes the in-game flag and, when it is set,
// each filled slot's def name; loading reads the flag into LoadedFromSaveGame and, when set, resizes the slots to
// min(names, max slots) and resolves each name.
#include <cstdio>
#include <cstring>

struct Archive { bool saving; bool savedInGame; int count; char names[4][16]; };
struct Aug { bool loaded; int maxSlots; int slotCount; int slots[4]; };
static const char* g_defNames[] = { "", "AUG_FIRE", "AUG_ICE", "AUG_BOLT" };
static int Lookup(const char* name) { for (int i = 1; i < 4; ++i) if (std::strcmp(g_defNames[i], name) == 0) return i; return 0; }
static void OnSerialise(Aug& a, Archive& ar, bool savedInGame)
{
    if (ar.saving) {
        ar.savedInGame = savedInGame;
        ar.count = a.slotCount;
        for (int i = 0; i < a.slotCount; ++i) ar.names[i][0] = 0;
        if (savedInGame)
            for (int i = 0; i < a.slotCount; ++i)
                if (a.slots[i]) std::strcpy(ar.names[i], g_defNames[a.slots[i]]);
    } else {
        a.loaded = ar.savedInGame;
        if (a.loaded) {
            long n = ar.count < a.maxSlots ? ar.count : a.maxSlots;
            a.slotCount = n;
            for (long i = 0; i < n; ++i) a.slots[i] = Lookup(ar.names[i]);
        }
    }
}
int main()
{
    Aug a; a.loaded = false; a.maxSlots = 2; a.slotCount = 3; a.slots[0] = 1; a.slots[1] = 0; a.slots[2] = 3;
    Archive ar; ar.saving = true;
    OnSerialise(a, ar, true);
    if (!ar.savedInGame || ar.count != 3 || std::strcmp(ar.names[0], "AUG_FIRE") != 0 || ar.names[1][0] != 0 ||
        std::strcmp(ar.names[2], "AUG_BOLT") != 0) { std::printf("00767200_TEST FAIL\n"); return 1; }
    Aug b; b.loaded = false; b.maxSlots = 2; b.slotCount = 0; b.slots[0] = b.slots[1] = b.slots[2] = 0;
    ar.saving = false;
    OnSerialise(b, ar, false);
    if (!b.loaded || b.slotCount != 2 || b.slots[0] != 1 || b.slots[1] != 0) { std::printf("00767200_TEST FAIL\n"); return 1; }
    ar.saving = true; OnSerialise(a, ar, false);
    if (ar.savedInGame || ar.names[0][0] != 0) { std::printf("00767200_TEST FAIL\n"); return 1; }
    std::printf("00767200_TEST PASS\n");
    return 0;
}