#include <cstdio>
struct SEntry { long key; long tag; long pad[2]; };
struct CLess { bool operator()(const SEntry& a, const SEntry& b) const { return a.key < b.key; } };
static int g_guarded = 0, g_unguarded = 0;
static void LinearInsert(SEntry* first, SEntry* last, SEntry val, CLess comp)
{
    if (comp(val, *first)) { for (SEntry* p = last; p != first; --p) *p = *(p - 1); *first = val; }
    else { SEntry* next = last - 1; while (comp(val, *next)) { *last = *next; last = next; --next; } *last = val; }
}
static void InsertionSort(SEntry* first, SEntry* last, CLess comp)
{
    ++g_guarded;
    if (first == last) return;
    for (SEntry* i = first + 1; i != last; ++i) LinearInsert(first, i, *i, comp);
}
static void UnguardedInsertionSortAux(SEntry* first, SEntry* last, SEntry*, CLess comp)
{
    ++g_unguarded;
    for (SEntry* i = first; i != last; ++i) {
        SEntry val = *i; SEntry* l = i; SEntry* next = l - 1;
        while (comp(val, *next)) { *l = *next; l = next; --next; }
        *l = val;
    }
}
static void FinalInsertionSort(SEntry* first, SEntry* last, CLess comp)
{
    if (last - first > 16) {
        InsertionSort(first, first + 16, comp);
        UnguardedInsertionSortAux(first + 16, last, 0, comp);
    }
    else
        InsertionSort(first, last, comp);
}
int main()
{
    bool ok = true;
    SEntry a[40];
    for (int i = 0; i < 40; ++i) { a[i].key = (i * 17) % 40; a[i].tag = i; a[i].pad[0] = a[i].pad[1] = 0; }
    a[3].key = -5;                                          // the minimum sits in the guarded head (a sentinel for the tail)
    FinalInsertionSort(a, a + 40, CLess());
    for (int i = 1; i < 40; ++i) if (a[i - 1].key > a[i].key) ok = false;
    if (g_guarded != 1 || g_unguarded != 1 || a[0].key != -5 || a[0].tag != 3) ok = false;
    SEntry b[5];
    for (int i = 0; i < 5; ++i) { b[i].key = 5 - i; b[i].tag = i; }
    g_guarded = g_unguarded = 0;
    FinalInsertionSort(b, b + 5, CLess());                  // short range: one guarded sort
    if (g_guarded != 1 || g_unguarded != 0 || b[0].key != 1 || b[4].key != 5) ok = false;
    if (!ok) { std::printf("004e415d_TEST FAIL\n"); return 1; }
    std::printf("004e415d_TEST PASS\n");
    return 0;
}