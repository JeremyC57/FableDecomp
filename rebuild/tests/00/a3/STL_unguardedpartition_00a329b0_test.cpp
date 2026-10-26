// Behaviour test for 0x00A329B0: the SGI heap / partition step over the keyed mesh-stats table, checked through the
// ordering it establishes (unsigned keys; the payload words travel with their keys).
#include <cstdio>
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
template <class T> inline void __iter_swap(TEntry* a, TEntry* b, T*) { T tmp = *a; *a = *b; *b = tmp; }
static TEntry* __unguarded_partition(TEntry* first, TEntry* last, TEntry pivot, CKeyPairCompareLess comp)
{
    for (;;) {
        while (comp(*first, pivot))
            ++first;
        --last;
        while (comp(pivot, *last))
            --last;
        if (!(first < last))
            return first;
        __iter_swap(first, last, (TEntry*)0);
        ++first;
    }
}
static int Fail() { std::printf("00a329b0_TEST FAIL\n"); return 1; }
int main()
{
    unsigned int keys[8] = { 6, 2, 9, 4, 1, 8, 3, 7 };
    TEntry a[8];
    for (int i = 0; i < 8; ++i) SetE(a[i], keys[i], i * 4);
    TEntry pivot; SetE(pivot, 5, 99);
    TEntry* cut = __unguarded_partition(a, a + 8, pivot, CKeyPairCompareLess());
    int k = (int)(cut - a);
    if (k != 4) return Fail();
    for (int i = 0; i < k; ++i) if (a[i].first >= 5 || !TagOk(a[i])) return Fail();
    for (int i = k; i < 8; ++i) if (a[i].first < 5 || !TagOk(a[i])) return Fail();
    std::printf("00a329b0_TEST PASS\n");
    return 0;
}