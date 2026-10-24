// Behaviour test for 0x0086C5C0: __linear_insert places *last's value into the sorted run [first, last): a value below the
// first entry shifts the run up one slot and goes first, otherwise it is sifted down from last (unsigned key).
#include <cstdio>

struct SValue { unsigned int Words[2]; };
struct SPair { unsigned int first; SValue second; };
struct SCompare { bool operator()(const SPair& a, const SPair& b) const { return a.first < b.first; } };

static SPair* copy_backward_model(SPair* first, SPair* last, SPair* result)
{
    for (int n = (int)(last - first); n > 0; --n) *--result = *--last;
    return result;
}

static void unguarded_linear_insert(SPair* last, SPair val, SCompare comp)
{
    SPair* next = last; --next;
    while (comp(val, *next)) { *last = *next; last = next; --next; }
    *last = val;
}

static void linear_insert(SPair* first, SPair* last, SPair val, SCompare comp)
{
    if (comp(val, *first)) { copy_backward_model(first, last, last + 1); *first = val; }
    else unguarded_linear_insert(last, val, comp);
}

static int Check(SPair* a, int n)
{
    for (int i = 1; i < n; ++i) if (a[i].first < a[i - 1].first) return 1;
    for (int i = 0; i < n; ++i) if (a[i].second.Words[0] != (unsigned int)a[i].first + 1000u) return 1;
    return 0;
}

int main()
{
    int fails = 0;
    if (sizeof(SPair) != 12u) ++fails;
    SPair a[5];
    unsigned int keys[5] = { 3, 5, 8, 9, 1 };            // the last entry is below the first
    for (int i = 0; i < 5; ++i) { a[i].first = keys[i]; a[i].second.Words[0] = (unsigned int)keys[i] + 1000u; a[i].second.Words[1] = 0; }
    linear_insert(a, a + 4, a[4], SCompare());
    if (Check(a, 5) || a[0].first != 1) ++fails;
    unsigned int keys2[5] = { 3, 5, 8, 9, 6 };           // the last entry belongs in the middle
    for (int i = 0; i < 5; ++i) { a[i].first = keys2[i]; a[i].second.Words[0] = (unsigned int)keys2[i] + 1000u; }
    linear_insert(a, a + 4, a[4], SCompare());
    if (Check(a, 5) || a[2].first != 6) ++fails;
    if (fails) { std::printf("0086c5c0_TEST FAIL\n"); return 1; }
    std::printf("0086c5c0_TEST PASS\n");
    return 0;
}