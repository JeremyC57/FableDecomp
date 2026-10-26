// behaviour test for 0x00448BE0 std::__make_heap<pair<EGameAction, CCountedPointer<const CInputTypeBase> >*, CKeyPairCompareLess<EGameAction, CCountedPointer<const CInputTypeBase> >, pair<...>, int>: every parent from (len - 2) / 2 down to the root is sifted down (STLport __adjust_heap), giving a max-heap by key with every reference count balanced; fewer than two entries is a no-op
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
static void MakeHeap(Entry* first, Entry* last)          // the function under test
{
    if (last - first < 2) return;
    int len = (int)(last - first);
    int parent = (len - 2) / 2;
    for (;;) {
        AdjustHeap(first, parent, len, Entry(first[parent]));
        if (parent == 0) return;
        parent--;
    }
}
int main()
{
    int fail = 0;
    Info inf[7];
    int keys[7] = { 10, 50, 20, 70, 30, 60, 40 };
    int i;
    for (i = 0; i < 7; ++i) { inf[i].RefCount = 0; inf[i].Deleted = 0; }
    {
        Entry h[7];
        for (i = 0; i < 7; ++i) { h[i].first = keys[i]; h[i].second = Counted(&keys[i], &inf[i]); }
        MakeHeap(h, h + 1);
        if (h[0].first != 10 || inf[0].RefCount != 1) fail |= 1;
        MakeHeap(h, h + 7);
        if (h[0].first != 70) fail |= 2;
        for (i = 1; i < 7; ++i) if (h[(i - 1) / 2].first < h[i].first) fail |= 4;
        for (i = 0; i < 7; ++i) if (*(int*)h[i].second.Data != h[i].first) fail |= 8;
        for (i = 0; i < 7; ++i) if (inf[i].RefCount != 1 || inf[i].Deleted) fail |= 16;
    }
    for (i = 0; i < 7; ++i) if (inf[i].RefCount != 0 || inf[i].Deleted != 1) fail |= 32;
    if (fail) { std::printf("00448be0_TEST FAIL %d\n", fail); return 1; }
    std::printf("00448be0_TEST PASS\n");
    return 0;
}