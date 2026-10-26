// Behaviour test for 0x00A32A70: the SGI heap / partition step over the keyed mesh-stats table, checked through the
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
static int Fail() { std::printf("00a32a70_TEST FAIL\n"); return 1; }
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
    std::printf("00a32a70_TEST PASS\n");
    return 0;
}