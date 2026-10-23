// Behaviour model of CTCInventoryAbilities::InitialiseAbilities (0x005EEDE2): bind every HERO_ABILITY def (index 1..n-1) to
// its ability slot (add-ref new, release old), clear the experience spent, then size the received-order list.
#include <cstdio>
struct Def { int refs; long ability; };
struct Entry { long level; Def* def; };
static void Assign(Def*& slot, Def* d) { if (slot != d) { if (slot) --slot->refs; slot = d; ++d->refs; } }
int main()
{
    Def defs[4] = { { 1, 0 }, { 1, 2 }, { 1, 0 }, { 1, 3 } };
    Entry abilities[4] = { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } };
    long spent[4] = { 5, 5, 5, 5 };
    long count = 4; unsigned long palettes = 6; unsigned long orderSize = 0;
    for (long i = 1; i < count; ++i) {
        Def* d = &defs[i]; ++d->refs;          // GetDef hands out a counted reference
        Assign(abilities[d->ability].def, d);
        spent[i] = 0;
        --d->refs;                               // local def pointer released
    }
    orderSize = palettes;
    int ok = abilities[2].def == &defs[1] && abilities[0].def == &defs[2] && abilities[3].def == &defs[3]
          && defs[1].refs == 2 && spent[0] == 5 && spent[1] == 0 && spent[3] == 0 && orderSize == 6;
    std::printf(ok ? "005eede2_TEST PASS\n" : "005eede2_TEST FAIL\n");
    return ok ? 0 : 1;
}