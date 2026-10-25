// Behaviour test for 0x007345D0 CTCCombat::GetAppropriateCombatAnimationFromList
// Model: three index lists (preferred = flourish or combo+handedness, handedness-only, combo-only); the pick is
// list[GFRandom % size] from the first non-empty list, else 0. No random number is drawn when every list is empty.
#include <cstdio>

namespace model {

enum { COMBO_STAGE_NONE = -777, COMBO_STAGE_FINISHER = -888, HANDEDNESS_EITHER = 2 };

struct Entry { int stage; int id; int start; bool flourish; };

unsigned int g_seed;

unsigned int ror13(unsigned int v) { return (v >> 13) | (v << 19); }
unsigned int GFRandom() { g_seed = g_seed * 0x24a1u + 0x24dfu; g_seed = ror13(g_seed); return g_seed; }
unsigned int GetRandomIndex(unsigned int range)
{
    unsigned int r = GFRandom();
    if (range == 0)
        return 0;
    return r % range;
}

int Select(const Entry* entries, int count, int chainPos, int chainID, int previous)
{
    int preferred[16]; int np = 0;
    int hand[16]; int nh = 0;
    int combo[16]; int nc = 0;
    for (int i = 0; i != count; ++i) {
        const Entry& e = entries[i];
        bool handOK = false;
        bool comboOK = false;
        if (previous == e.start || e.start == HANDEDNESS_EITHER)
            handOK = true;
        int stage = e.stage;
        if (chainPos == COMBO_STAGE_NONE) {
            if (stage == 0 || stage == chainPos || stage == COMBO_STAGE_FINISHER)
                comboOK = true;
        }
        else if (chainID == e.id) {
            if (stage == chainPos || stage == COMBO_STAGE_FINISHER)
                comboOK = true;
        }
        if (e.flourish || (comboOK && handOK))
            preferred[np++] = i;
        if (handOK)
            hand[nh++] = i;
        if (comboOK)
            combo[nc++] = i;
    }
    if (np)
        return preferred[GetRandomIndex((unsigned int)np)];
    if (nh)
        return hand[GetRandomIndex((unsigned int)nh)];
    if (nc)
        return combo[GetRandomIndex((unsigned int)nc)];
    return 0;
}

int run()
{
    int bad = 0;
    // 1. Empty list: 0 and the seed untouched.
    g_seed = 1234;
    if (Select(0, 0, COMBO_STAGE_NONE, 0, 0) != 0 || g_seed != 1234) bad = 1;

    // 2. Nothing fits (wrong handedness, stage 3 with no chain): 0 and no draw.
    Entry none[2] = { { 3, 0, 1, false }, { 4, 0, 1, false } };
    g_seed = 99;
    if (Select(none, 2, COMBO_STAGE_NONE, 0, 0) != 0 || g_seed != 99) bad = 2;

    // 3. Exactly one full match (stage 0, previous handedness) among misfits: always that index, one draw.
    Entry one[3] = { { 3, 0, 1, false }, { 0, 0, 0, false }, { 5, 0, 1, false } };
    g_seed = 7;
    if (Select(one, 3, COMBO_STAGE_NONE, 0, 0) != 1) bad = 3;
    unsigned int expect = 7u * 0x24a1u + 0x24dfu;
    expect = ror13(expect);
    if (g_seed != expect) bad = 4;

    // 4. A flourish entry is preferred even when nothing else fits.
    Entry fl[3] = { { 3, 0, 1, false }, { 3, 0, 1, true }, { 0, 0, 1, false } };
    if (Select(fl, 3, COMBO_STAGE_NONE, 0, 0) != 1) bad = 5;

    // 5. Handedness-only fallback: EITHER handedness, stage mismatched.
    Entry ho[2] = { { 3, 0, 1, false }, { 3, 0, HANDEDNESS_EITHER, false } };
    if (Select(ho, 2, COMBO_STAGE_NONE, 0, 0) != 1) bad = 6;

    // 6. Combo-only fallback: in a chain (pos 2, id 9), entry 1 has the right id and is a finisher, wrong hand.
    Entry co[3] = { { 2, 8, 1, false }, { COMBO_STAGE_FINISHER, 9, 1, false }, { 1, 9, 1, false } };
    if (Select(co, 3, 2, 9, 0) != 1) bad = 7;

    // 7. Chain id mismatch never combo-matches; stage == pos with the right id does.
    Entry ch[2] = { { 2, 8, 0, false }, { 2, 9, 0, false } };
    if (Select(ch, 2, 2, 9, 0) != 1) bad = 8;   // entry 0 is handedness-only, entry 1 is preferred

    // 8. Several preferred entries: the pick is preferred[r % n] for the advanced seed.
    Entry many[4] = { { 0, 0, 0, false }, { 3, 0, 1, false }, { COMBO_STAGE_FINISHER, 0, HANDEDNESS_EITHER, false }, { 0, 0, 0, false } };
    g_seed = 555;
    unsigned int s = 555u * 0x24a1u + 0x24dfu;
    s = ror13(s);
    const int pref[3] = { 0, 2, 3 };
    int want = pref[s % 3];
    if (Select(many, 4, COMBO_STAGE_NONE, 0, 0) != want) bad = 9;
    return bad;
}

}

int main()
{
    int bad = model::run();
    if (bad) { std::printf("007345d0_TEST FAIL %d\n", bad); return 1; }
    std::printf("007345d0_TEST PASS\n");
    return 0;
}