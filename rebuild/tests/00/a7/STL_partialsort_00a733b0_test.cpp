// Behaviour test for 0x00A733B0 std::__partial_sort over (CCharString key, record) pairs: the key compare is the
// inline CCharString::operator< (same data -> false, null left -> true, null right -> false, else the data compare).
#include <cstdio>

static bool DataLess(const char* a, const char* b)       // CBasicString<char>::operator< (0x00429950)
{
    for (;;) {
        if (*a == 0 && *b == 0) return false;
        if (*a < *b) return true;
        if (*a > *b) return false;
        ++a; ++b;
    }
}
struct SRec { const char* key; int tag; };
struct CLess {
    bool operator()(const SRec& a, const SRec& b) const
    {
        if (b.key == a.key) return false;
        if (a.key == 0) return true;
        if (b.key == 0) return false;
        return DataLess(a.key, b.key);
    }
};
static void AdjustHeap(SRec* first, int hole, int len, SRec val, CLess comp)
{
    int top = hole;
    int child = 2 * hole + 2;
    while (child < len) {
        if (comp(first[child], first[child - 1])) child--;
        first[hole] = first[child];
        hole = child;
        child = 2 * (child + 1);
    }
    if (child == len) { first[hole] = first[child - 1]; hole = child - 1; }
    int parent = (hole - 1) / 2;
    while (hole > top && comp(first[parent], val)) {
        first[hole] = first[parent];
        hole = parent;
        parent = (hole - 1) / 2;
    }
    first[hole] = val;
}
static void MakeHeap(SRec* first, SRec* last, CLess comp)
{
    int len = (int)(last - first);
    if (len < 2) return;
    for (int parent = (len - 2) / 2; ; parent--) {
        AdjustHeap(first, parent, len, first[parent], comp);
        if (parent == 0) return;
    }
}
static void PopHeap(SRec* first, SRec* last, SRec* result, SRec val, CLess comp)
{
    *result = *first;
    AdjustHeap(first, 0, (int)(last - first), val, comp);
}
static void SortHeap(SRec* first, SRec* last, CLess comp)
{
    while (last - first > 1) { --last; PopHeap(first, last, last, *last, comp); }
}
static void PartialSort(SRec* first, SRec* middle, SRec* last, SRec*, CLess comp)
{
    MakeHeap(first, middle, comp);
    for (SRec* i = middle; i < last; ++i)
        if (comp(*i, *first))
            PopHeap(first, middle, i, SRec(*i), comp);
    SortHeap(first, middle, comp);
}

int main()
{
    const char* keys[9] = { "pear", "apple", 0, "fig", "zebra", "banana", "apple2", "kiwi", "cherry" };
    SRec v[9];
    for (int i = 0; i < 9; ++i) { v[i].key = keys[i]; v[i].tag = i; }
    PartialSort(v, v + 4, v + 9, (SRec*)0, CLess());
    if (v[0].key != 0) { std::printf("00a733b0_TEST FAIL\n"); return 1; }
    const char* expect[3] = { "apple", "apple2", "banana" };
    for (int i = 0; i < 3; ++i) {
        const char* a = v[i + 1].key; const char* b = expect[i];
        while (*a && *a == *b) { ++a; ++b; }
        if (*a != *b) { std::printf("00a733b0_TEST FAIL\n"); return 1; }
    }
    std::printf("00a733b0_TEST PASS\n");
    return 0;
}