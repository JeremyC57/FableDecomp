// behaviour test for 0x006BAD60 std::__insertion_sort<pair<unsigned long, CCountedPointer<CFaction> >*, CKeyPairCompareLess<unsigned long, CCountedPointer<CFaction> > >: each entry after the first is linearly inserted into the sorted run before it (STLport __linear_insert), giving a stable ascending order by key with every reference count balanced
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
static void LinearInsert(Entry* first, Entry* last, Entry val)
{
    if (Less(val, *first)) {
        for (Entry* p = last; p != first; --p) p[0] = p[-1];
        *first = val;
    } else {
        Entry* next = last - 1;
        while (Less(val, *next)) { *last = *next; last = next; --next; }
        *last = val;
    }
}
static void InsertionSort(Entry* first, Entry* last)    // the function under test
{
    if (first == last) return;
    for (Entry* i = first + 1; i != last; ++i)
        LinearInsert(first, i, *i);
}
int main()
{
    int fail = 0;
    Info inf[7];
    int keys[7] = { 40, 10, 70, 10, 30, 5, 60 };
    int i;
    for (i = 0; i < 7; ++i) { inf[i].RefCount = 0; inf[i].Deleted = 0; }
    {
        Entry h[7];
        for (i = 0; i < 7; ++i) { h[i].first = keys[i]; h[i].second = Counted(&keys[i], &inf[i]); }
        InsertionSort(h, h);
        if (h[0].first != 40) fail |= 1;
        InsertionSort(h, h + 7);
        for (i = 1; i < 7; ++i) if (h[i].first < h[i - 1].first) fail |= 2;
        if (h[0].first != 5 || h[6].first != 70) fail |= 4;
        if (h[1].second.Data != &keys[1] || h[2].second.Data != &keys[3]) fail |= 8;   // equal keys keep their order
        for (i = 0; i < 7; ++i) if (inf[i].RefCount != 1 || inf[i].Deleted) fail |= 16;
    }
    for (i = 0; i < 7; ++i) if (inf[i].RefCount != 0 || inf[i].Deleted != 1) fail |= 32;
    if (fail) { std::printf("006bad60_TEST FAIL %d\n", fail); return 1; }
    std::printf("006bad60_TEST PASS\n");
    return 0;
}