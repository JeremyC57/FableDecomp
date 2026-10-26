// Behaviour test for 0x00A32500: the SGI heap / partition step over the keyed mesh-stats table, checked through the
// ordering it establishes (unsigned keys; the payload words travel with their keys).
#include <cstdio>
class CMatrix3x4 { public: float M[12]; };
class C3DDummyObject { public: CMatrix3x4 Transform; int BoneIndex; };
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair() { }
    pair(const pair& p) : first(p.first), second(p.second) { }
};
typedef pair<unsigned int, C3DDummyObject> TEntry;
static void SetE(TEntry& e, unsigned int key, int tag) { e.first = key; e.second.BoneIndex = tag; for (int j = 0; j < 12; ++j) e.second.Transform.M[j] = (float)(tag + j); }
static bool TagOk(const TEntry& e) { float d = e.second.Transform.M[5] - (float)(e.second.BoneIndex + 5); return d < 0.001f && d > -0.001f; }
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
        if (comp(*(first + secondChild), *(first + (secondChild - 1))))
            secondChild--;
        *(first + holeIndex) = *(first + secondChild);
        holeIndex = secondChild;
        secondChild = 2 * (secondChild + 1);
    }
    if (secondChild == len) {
        *(first + holeIndex) = *(first + (secondChild - 1));
        holeIndex = secondChild - 1;
    }
    __push_heap(first, holeIndex, topIndex, val, comp);
}
static void __make_heap(TEntry* first, TEntry* last, CKeyPairCompareLess comp, TEntry*, int*)
{
    if (last - first < 2)
        return;
    int len = (int)(last - first);
    int parent = (len - 2) / 2;
    for (;;) {
        __adjust_heap(first, parent, len, TEntry(*(first + parent)), comp);
        if (parent == 0)
            return;
        parent--;
    }
}
static bool IsHeap(const TEntry* a, int n)
{
    for (int i = 1; i < n; ++i) if (a[(i - 1) / 2].first < a[i].first) return false;
    for (int i = 0; i < n; ++i) if (!TagOk(a[i])) return false;
    return true;
}
static int Fail() { std::printf("00a32500_TEST FAIL\n"); return 1; }
int main()
{
    unsigned int keys[9] = { 5, 1, 9, 3, 7, 0x90000000u, 2, 8, 4 };
    TEntry a[9];
    for (int i = 0; i < 9; ++i) SetE(a[i], keys[i], i * 3);
    __make_heap(a, a + 9, CKeyPairCompareLess(), (TEntry*)0, (int*)0);
    if (!IsHeap(a, 9) || a[0].first != 0x90000000u || a[0].second.BoneIndex != 15) return Fail();
    // pop: move the last element into the root hole via __adjust_heap
    TEntry v(a[8]); a[8] = a[0];
    __adjust_heap(a, 0, 8, v, CKeyPairCompareLess());
    if (!IsHeap(a, 8) || a[0].first != 9 || a[8].first != 0x90000000u) return Fail();
    std::printf("00a32500_TEST PASS\n");
    return 0;
}