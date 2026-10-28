// Behaviour test for 0x006D72D0 CTCHeroExperience::GetActiveComboMultiplierData() const: the rounded multiplier
// (clamped to at least 1) selects the first entry whose [previous key, key) bracket holds it, else the last entry.
#include <cstdio>
struct Entry { long Key; int Data; };
const int* GetActive(float param, float per_unit, const Entry* entries, int count)
{
    long multiplier = (long)(param / per_unit + 0.5);
    if (multiplier < 1) multiplier = 1;
    long previous = entries[0].Key;
    for (int i = 1; i < count; ++i) {
        long key = entries[i].Key;
        if (multiplier >= previous && multiplier < key) return &entries[i].Data;
        previous = key;
    }
    return &entries[count - 1].Data;
}
int main()
{
    Entry e[4] = { { 1, 10 }, { 3, 20 }, { 6, 30 }, { 10, 40 } };
    bool ok = *GetActive(0.0f, 1.0f, e, 4) == 20;        // clamped to 1 -> [1,3)
    ok = ok && *GetActive(2.4f, 1.0f, e, 4) == 20;       // 2 -> [1,3)
    ok = ok && *GetActive(2.6f, 1.0f, e, 4) == 30;       // 3 -> [3,6)
    ok = ok && *GetActive(9.0f, 1.0f, e, 4) == 40;       // 9 -> [6,10)
    ok = ok && *GetActive(50.0f, 1.0f, e, 4) == 40;      // past the end -> last
    ok = ok && *GetActive(8.0f, 2.0f, e, 4) == 30;       // 4 -> [3,6)
    if (!ok) { std::printf("006d72d0_TEST FAIL\n"); return 1; }
    std::printf("006d72d0_TEST PASS\n");
    return 0;
}