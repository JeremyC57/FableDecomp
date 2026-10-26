// behaviour test for 0x00A81F10 std::__pop_heap<pair<long, CCountedPointer<CNavConnectorNode> >*, pair<...>, CKeyPairCompareLess<long, CCountedPointer<CNavConnectorNode> >, int>: the heap top lands in result, the saved value is sifted back into the heap (STLport __adjust_heap), the heap property holds, and every reference count is balanced
#include <cstdio>
struct Info { int RefCount; int Deleted; };
struct Counted {
    void* Data; Info* I;
    Counted() : Data(0), I(0) { }
    Counted(void* d, Info* i) : Data(d), I(i) { if (I) ++I->RefCount; }
    Counted(const Counted& o) : Data(o.Data), I(o.I) { if (I) ++I->RefCount; }
    ~Counted() { Release(); }
    void Release() { if (I) { if (--I->RefCount == 0) ++I->Deleted; } }
    Counted& operator=(const Counted& o) { if (I != o.I) { Release(); Data = o.Data; I = o.I; if (I) ++I->RefCount; } return *this; }
};
struct Entry { int first; Counted second; };
static bool Less(const Entry& a, const Entry& b) { return a.first < b.first; }
static void AdjustHeap(Entry* first, int hole, int len, Entry val)
{
    int top = hole, child = 2 * hole + 2;
    while (child < len) {
        if (Less(first[child], first[child - 1])) child--;
        first[hole] = first[child]; hole = child; child = 2 * (child + 1);
    }
    if (child == len) { first[hole] = first[child - 1]; hole = child - 1; }
    int parent = (hole - 1) / 2;
    while (hole > top && Less(first[parent], val)) { first[hole] = first[parent]; hole = parent; parent = (hole - 1) / 2; }
    first[hole] = val;
}
static void PopHeap(Entry* first, Entry* last, Entry* result, Entry val)   // the function under test
{
    *result = *first;
    AdjustHeap(first, 0, (int)(last - first), val);
}
int main()
{
    int fail = 0;
    Info inf[6];
    int keys[6] = { 90, 70, 80, 10, 60, 50 };              // a max-heap
    int i;
    for (i = 0; i < 6; ++i) { inf[i].RefCount = 0; inf[i].Deleted = 0; }
    {
        Entry h[6];
        for (i = 0; i < 6; ++i) { h[i].first = keys[i]; h[i].second = Counted(&keys[i], &inf[i]); }
        Entry v = h[5];                                    // __pop_heap_aux: value = *(last - 1), result = last - 1
        PopHeap(h, h + 5, h + 5, v);
        if (h[5].first != 90 || h[5].second.I != &inf[0]) fail |= 1;
        if (h[0].first != 80) fail |= 2;
        for (i = 1; i < 5; ++i) if (h[(i - 1) / 2].first < h[i].first) fail |= 4;
        int seen = 0;
        for (i = 0; i < 6; ++i) seen += h[i].first;
        if (seen != 90 + 70 + 80 + 10 + 60 + 50) fail |= 8;
        if (inf[5].RefCount != 2 || inf[0].RefCount != 1 || inf[0].Deleted) fail |= 16;
    }
    for (i = 0; i < 6; ++i) if (inf[i].RefCount != 0 || inf[i].Deleted != 1) fail |= 32;
    if (fail) { std::printf("00a81f10_TEST FAIL %d\n", fail); return 1; }
    std::printf("00a81f10_TEST PASS\n");
    return 0;
}