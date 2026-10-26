// Behaviour test for 0x00A342F0 std::__partial_sort over the 20-byte helper-point entries: the five smallest keys sorted in front, payloads intact.
#include <cstdio>
#include <new>
class C3DVector { public: float X, Y, Z; C3DVector() { } C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { } };
class C3DHelperPoint { public: C3DVector Position; int BoneIndex; };
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair() { }
    pair(const pair& p) : first(p.first), second(p.second) { }
};
typedef pair<unsigned int, C3DHelperPoint> TEntry;
static void SetE(TEntry& e, unsigned int key, int tag) { e.first = key; e.second.BoneIndex = tag; e.second.Position.X = (float)tag; e.second.Position.Y = 0.5f; e.second.Position.Z = (float)(tag * 2); }
static bool TagOk(const TEntry& e) { float d = e.second.Position.Z - (float)(e.second.BoneIndex * 2); return d < 0.001f && d > -0.001f; }
struct CKeyPairCompareLess {
    bool operator()(const TEntry& a, const TEntry& b) const { return a.first < b.first; }
};
static void __push_heap(TEntry* first, int holeIndex, int topIndex, TEntry val, CKeyPairCompareLess comp)
{
    int parent = (holeIndex - 1) / 2;
    while (holeIndex > topIndex && comp(*(first + parent), val)) {
        *(first + holeIndex) = *(first + parent);
        holeIndex = parent;
        parent = (holeIndex - 1) / 2;
    }
    *(first + holeIndex) = val;
}
static void __adjust_heap(TEntry* first, int holeIndex, int len, TEntry val, CKeyPairCompareLess comp)
{
    int topIndex = holeIndex;
    int secondChild = 2 * holeIndex + 2;
    while (secondChild < len) {
        if (comp(*(first + secondChild), *(first + (secondChild - 1)))) secondChild--;
        *(first + holeIndex) = *(first + secondChild);
        holeIndex = secondChild;
        secondChild = 2 * (secondChild + 1);
    }
    if (secondChild == len) { *(first + holeIndex) = *(first + (secondChild - 1)); holeIndex = secondChild - 1; }
    __push_heap(first, holeIndex, topIndex, val, comp);
}
static void __make_heap(TEntry* first, TEntry* last, CKeyPairCompareLess comp, TEntry*, int*)
{
    if (last - first < 2) return;
    int len = (int)(last - first);
    int parent = (len - 2) / 2;
    for (;;) {
        __adjust_heap(first, parent, len, TEntry(*(first + parent)), comp);
        if (parent == 0) return;
        parent--;
    }
}
static void __pop_heap(TEntry* first, TEntry* last, TEntry* result, TEntry value, CKeyPairCompareLess comp, int*)
{
    *result = *first;
    __adjust_heap(first, 0, (int)(last - first), value, comp);
}
static void sort_heap(TEntry* first, TEntry* last, CKeyPairCompareLess comp)
{
    while (last - first > 1) { --last; __pop_heap(first, last, last, TEntry(*last), comp, (int*)0); }
}
static void __partial_sort(TEntry* first, TEntry* middle, TEntry* last, TEntry*, CKeyPairCompareLess comp)
{
    __make_heap(first, middle, comp, (TEntry*)0, (int*)0);
    for (TEntry* i = middle; i < last; ++i)
        if (comp(*i, *first))
            __pop_heap(first, middle, i, TEntry(*i), comp, (int*)0);
    sort_heap(first, middle, comp);
}
static int Fail() { std::printf("00a342f0_TEST FAIL\n"); return 1; }

int main()
{
    unsigned int keys[12] = { 50, 3, 99, 7, 1, 64, 20, 0x80000000u, 5, 42, 8, 2 };
    TEntry a[12];
    for (int i = 0; i < 12; ++i) SetE(a[i], keys[i], i * 3);
    __partial_sort(a, a + 5, a + 12, (TEntry*)0, CKeyPairCompareLess());
    unsigned int want[5] = { 1, 2, 3, 5, 7 };
    int wantTag[5] = { 12, 33, 3, 24, 9 };
    for (int i = 0; i < 5; ++i) if (a[i].first != want[i] || a[i].second.BoneIndex != wantTag[i]) return Fail();
    for (int i = 0; i < 12; ++i) if (!TagOk(a[i]) || (i >= 5 && a[i].first < 7)) return Fail();
    std::printf("00a342f0_TEST PASS\n");
    return 0;
}