// Behaviour test for 00a32b50: out-of-line __pop_heap for pair<unsigned long, C3DHelperPoint> -- heap-sort seven keyed elements through the row's function (with the
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

class C3DHelperPoint { public:                                // 0x10 bytes
    C3DVector Position;                                // +0x00
    long      BoneIndex;                               // +0x0c
};

struct SKeyedHelperPoint {                             // std::pair<unsigned long, C3DHelperPoint>, 20 bytes
    unsigned long  first;                              // +0x00
    C3DHelperPoint second;                             // +0x04
    SKeyedHelperPoint() { }
    SKeyedHelperPoint(const SKeyedHelperPoint& other) : first(other.first), second(other.second) { }
};

struct CKeyPairCompareLess {                           // empty, passed by value
    CKeyPairCompareLess() { }
    bool operator()(const SKeyedHelperPoint& a, const SKeyedHelperPoint& b) const { return a.first < b.first; }
};

void __adjust_heap(SKeyedHelperPoint* first, long holeIndex, long len, SKeyedHelperPoint value,
                              CKeyPairCompareLess comp) { adjust_impl(first, holeIndex, len, value, comp); }   // 0x00A32440

void __pop_heap(SKeyedHelperPoint* first, SKeyedHelperPoint* last, SKeyedHelperPoint* result,
                           SKeyedHelperPoint value, CKeyPairCompareLess comp, long*)
{
    *result = *first;
    __adjust_heap(first, 0, long(last - first), value, comp);
}

#define KEY(e) ((long)(e).first)
int main()
{
    {
        static const int keys[7] = { 5, 9, 1, 7, 3, 8, 2 };
        static const int sorted[7] = { 1, 2, 3, 5, 7, 8, 9 };
        SKeyedHelperPoint a[7];
        for (int i = 0; i < 7; ++i) {
            SKeyedHelperPoint& e = a[i];
            int k = keys[i];
            e.first = (unsigned long)k;
            e.second.BoneIndex = k * 10; e.second.Position.Z = (float)k;
        }
        CKeyPairCompareLess comp;
        for (int i = 1; i < 7; ++i) {
            SKeyedHelperPoint v(a[i]);
            push_impl(a, (long)i, 0L, v, comp);
        }
        for (int len = 7; len > 1; --len)
            __pop_heap(a, a + len - 1, a + len - 1, SKeyedHelperPoint(a[len - 1]), comp, (long*)0);
        for (int i = 0; i < 7; ++i) {
            const SKeyedHelperPoint& e = a[i];
            if (KEY(e) != sorted[i] || !(e.second.BoneIndex == KEY(e) * 10 && e.second.Position.Z == (float)KEY(e))) { std::printf("00a32b50_TEST FAIL\n"); return 1; }
        }
    }
    std::printf("00a32b50_TEST PASS\n");
    return 0;
}