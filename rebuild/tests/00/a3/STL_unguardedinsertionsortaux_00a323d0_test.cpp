// Behaviour test for 0x00A323D0 std::__unguarded_insertion_sort_aux over pair<unsigned long, C3DDummyObject>: with a
// sentinel minimum in front of the range, the pass sorts by unsigned key and keeps equal keys in their original order.
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
struct CKeyPairCompareLess {
    bool operator()(const TEntry& a, const TEntry& b) const { return a.first < b.first; }
};
template <class RandomAccessIter, class T, class Compare>
inline void __unguarded_linear_insert(RandomAccessIter last, T val, Compare comp)
{
    RandomAccessIter next = last;
    --next;
    while (comp(val, *next)) {
        *last = *next;
        last = next;
        --next;
    }
    *last = val;
}
static void __unguarded_insertion_sort_aux(TEntry* first, TEntry* last, TEntry*, CKeyPairCompareLess comp)
{
    for (TEntry* i = first; i != last; ++i)
        __unguarded_linear_insert(i, TEntry(*i), comp);
}
int main()
{
    unsigned int keys[7] = { 0, 9, 4, 0x80000000u, 4, 1, 7 };
    TEntry a[7];
    for (int i = 0; i < 7; ++i) {
        a[i].first = keys[i];
        a[i].second.BoneIndex = i;
        for (int j = 0; j < 12; ++j) a[i].second.Transform.M[j] = (float)(i * 10 + j);
    }
    __unguarded_insertion_sort_aux(a + 1, a + 7, (TEntry*)0, CKeyPairCompareLess());
    unsigned int wantKey[7] = { 0, 1, 4, 4, 7, 9, 0x80000000u };
    int wantBone[7] = { 0, 5, 2, 4, 6, 1, 3 };
    for (int i = 0; i < 7; ++i) {
        if (a[i].first != wantKey[i] || a[i].second.BoneIndex != wantBone[i]) { std::printf("00a323d0_TEST FAIL\n"); return 1; }
        float d = a[i].second.Transform.M[11] - (float)(wantBone[i] * 10 + 11);
        if (d > 0.001f || d < -0.001f) { std::printf("00a323d0_TEST FAIL\n"); return 1; }
    }
    std::printf("00a323d0_TEST PASS\n");
    return 0;
}