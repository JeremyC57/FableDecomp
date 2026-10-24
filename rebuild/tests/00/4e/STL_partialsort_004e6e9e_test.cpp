#include <cstdio>
#include <cstring>
struct SEntry { char key[8]; long value; };
struct CLess { bool operator()(const SEntry& a, const SEntry& b) const { return std::strcmp(a.key, b.key) < 0; } };
static void PushHeap(SEntry* first, int hole, int top, SEntry val, CLess comp)
{
    int parent = (hole - 1) / 2;
    while (hole > top && comp(first[parent], val)) { first[hole] = first[parent]; hole = parent; parent = (hole - 1) / 2; }
    first[hole] = val;
}
static void AdjustHeap(SEntry* first, int hole, int len, SEntry val, CLess comp)
{
    int top = hole, second = 2 * hole + 2;
    while (second < len) {
        if (comp(first[second], first[second - 1])) second--;
        first[hole] = first[second]; hole = second; second = 2 * (second + 1);
    }
    if (second == len) { first[hole] = first[second - 1]; hole = second - 1; }
    PushHeap(first, hole, top, val, comp);
}
static void MakeHeap(SEntry* first, SEntry* last, CLess comp)
{
    if (last - first < 2) return;
    int len = (int)(last - first), parent = (len - 2) / 2;
    while (true) { AdjustHeap(first, parent, len, first[parent], comp); if (parent == 0) return; parent--; }
}
static void PopHeap(SEntry* first, SEntry* last, SEntry* result, SEntry val, CLess comp, int*)
{
    *result = *first;
    AdjustHeap(first, 0, (int)(last - first), val, comp);
}
static void SortHeap(SEntry* first, SEntry* last, CLess comp)
{
    while (last - first > 1) { --last; SEntry v = *last; PopHeap(first, last, last, v, comp, 0); }
}
static void PartialSort(SEntry* first, SEntry* middle, SEntry* last, SEntry*, CLess comp)
{
    MakeHeap(first, middle, comp);
    for (SEntry* i = middle; i < last; ++i)
        if (comp(*i, *first))
            PopHeap(first, middle, i, SEntry(*i), comp, 0);
    SortHeap(first, middle, comp);
}
int main()
{
    const char* keys[9] = { "kilo", "alfa", "zulu", "echo", "bravo", "yank", "delta", "lima", "char" };
    SEntry a[9];
    for (int i = 0; i < 9; ++i) { std::strcpy(a[i].key, keys[i]); a[i].value = i; }
    PartialSort(a, a + 4, a + 9, 0, CLess());
    const char* want[4] = { "alfa", "bravo", "char", "delta" };
    bool ok = true;
    for (int i = 0; i < 4; ++i) if (std::strcmp(a[i].key, want[i])) ok = false;
    if (a[0].value != 1 || a[3].value != 6) ok = false;
    long sum = 0; for (int i = 0; i < 9; ++i) sum += a[i].value;
    if (sum != 36) ok = false;                              // a permutation: nothing lost
    if (!ok) { std::printf("004e6e9e_TEST FAIL\n"); return 1; }
    std::printf("004e6e9e_TEST PASS\n");
    return 0;
}