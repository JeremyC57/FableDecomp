// Behaviour test for 00808db0: __pop_heap_aux for pair<C3DVector, long> (test comparator orders by X) -- heap-sort seven keyed elements through the row's function (with the
// SGI __adjust_heap/__push_heap written out here) and check ascending keys with each payload still beside its key.
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __cdecl
#endif
static const char* const g_names[10] = { "k0", "k1", "k2", "k3", "k4", "k5", "k6", "k7", "k8", "k9" };
#define NUM(s) ((long)((s).c_str()[1] - '0'))
template <class T, class C> void push_impl(T* first, long holeIndex, long topIndex, T value, C comp)
{
    long parent = (holeIndex - 1) / 2;
    while (holeIndex > topIndex && comp(first[parent], value)) {
        first[holeIndex] = first[parent];
        holeIndex = parent;
        parent = (holeIndex - 1) / 2;
    }
    first[holeIndex] = value;
}
template <class T, class C> void adjust_impl(T* first, long holeIndex, long len, T value, C comp)
{
    long topIndex = holeIndex;
    long secondChild = 2 * holeIndex + 2;
    while (secondChild < len) {
        if (comp(first[secondChild], first[secondChild - 1]))
            secondChild--;
        first[holeIndex] = first[secondChild];
        holeIndex = secondChild;
        secondChild = 2 * (secondChild + 1);
    }
    if (secondChild == len) {
        first[holeIndex] = first[secondChild - 1];
        holeIndex = secondChild - 1;
    }
    push_impl(first, holeIndex, topIndex, value, comp);
}
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }   // memberwise user copy (no user operator=)
};

struct PairVectorLong {                                // std::pair<C3DVector, long>, 16 bytes
    C3DVector first;                                   // +0x00
    long      second;                                  // +0x0c
    PairVectorLong() { }
    PairVectorLong(const PairVectorLong& other) : first(other.first), second(other.second) { }
};

class CVectorCompare_BestForTargeting { public:               // 0x2c bytes (Ego_r.pdb, matches retail)
    C3DVector AnchorPos;                               // +0x00
    C3DVector PreferredLine;                           // +0x0c
    float     AngleCompare;                            // +0x18
    float     SqrDistCompare;                          // +0x1c
    float     OverrideShortDistanceFactor;             // +0x20
    float     OverrideShortDistanceFalloff;            // +0x24
    bool      CompareInXYPlane;                        // +0x28
};

struct CCompareTargetedPositionsForFirstPersonTargeting {      // anonymous namespace in retail, 0x2c bytes
    CVectorCompare_BestForTargeting VectorCompare;             // +0x00
    bool operator()(const PairVectorLong& a, const PairVectorLong& b) const { return a.first.X < b.first.X; }
};

void __pop_heap(PairVectorLong* first, PairVectorLong* last, PairVectorLong* result, PairVectorLong value,
                           CCompareTargetedPositionsForFirstPersonTargeting comp, long*) { *result = *first; adjust_impl(first, 0L, long(last - first), value, comp); }   // 0x00808BD0

void __pop_heap_aux(PairVectorLong* first, PairVectorLong* last, PairVectorLong*,
                               CCompareTargetedPositionsForFirstPersonTargeting comp)
{
    __pop_heap(first, last - 1, last - 1, PairVectorLong(*(last - 1)), comp, (long*)0);
}

#define KEY(e) ((long)(e).first.X)
int main()
{
    {
        static const int keys[7] = { 5, 9, 1, 7, 3, 8, 2 };
        static const int sorted[7] = { 1, 2, 3, 5, 7, 8, 9 };
        PairVectorLong a[7];
        for (int i = 0; i < 7; ++i) {
            PairVectorLong& e = a[i];
            int k = keys[i];
            e.first.X = (float)k; e.first.Y = 0.0f; e.first.Z = 0.0f;
            e.second = k * 10;
        }
        CCompareTargetedPositionsForFirstPersonTargeting comp;
        for (int i = 1; i < 7; ++i) {
            PairVectorLong v(a[i]);
            push_impl(a, (long)i, 0L, v, comp);
        }
        for (int len = 7; len > 1; --len)
            __pop_heap_aux(a, a + len, (PairVectorLong*)0, comp);
        for (int i = 0; i < 7; ++i) {
            const PairVectorLong& e = a[i];
            if (KEY(e) != sorted[i] || !(e.second == KEY(e) * 10)) { std::printf("00808db0_TEST FAIL\n"); return 1; }
        }
    }
    std::printf("00808db0_TEST PASS\n");
    return 0;
}