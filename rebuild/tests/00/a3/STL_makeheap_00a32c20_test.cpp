// Behaviour test for 0x00A32C20 std::__make_heap over pair<unsigned long, C3DHelperPoint> by the unsigned key (the SGI __adjust_heap / __push_heap pair stands in for 0x00A32440).
#include <cstdio>

class C3DVector { public:
    float X, Y, Z;
    C3DVector() : X(0), Y(0), Z(0) { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};
class C3DHelperPoint { public: C3DVector Position; long BoneIndex; };
template <class T1, class T2> struct pair { T1 first; T2 second; };
template <class K, class V> class CKeyPairCompareLess { public:
    bool operator()(const pair<K, V>& a, const pair<K, V>& b) const { return a.first < b.first; }
};
typedef pair<unsigned long, C3DHelperPoint> T;
typedef CKeyPairCompareLess<unsigned long, C3DHelperPoint> P;

static int g_adjusts = 0;
static void __push_heap(T* first, int holeIndex, int topIndex, T value, P comp)
{
    int parent = (holeIndex - 1) / 2;
    while (holeIndex > topIndex && comp(*(first + parent), value)) {
        *(first + holeIndex) = *(first + parent);
        holeIndex = parent;
        parent = (holeIndex - 1) / 2;
    }
    *(first + holeIndex) = value;
}
static void __adjust_heap(T* first, int holeIndex, int len, T value, P comp)
{
    ++g_adjusts;
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
    __push_heap(first, holeIndex, topIndex, value, comp);
}
static void __make_heap(T* first, T* last, P comp, T*, int*)
{
    if (last - first < 2)
        return;
    int len = last - first;
    int parent = (len - 2) / 2;
    for (;;) {
        __adjust_heap(first, parent, len, T(*(first + parent)), comp);
        if (parent == 0)
            return;
        parent--;
    }
}

int main()
{
    unsigned long keys[6] = { 3, 0x80000001UL, 7, 2, 0xFFFFFFF0UL, 7 };   // high keys check the unsigned order
    T a[6];
    for (int k = 0; k < 6; ++k) { a[k].first = keys[k]; a[k].second.BoneIndex = k; a[k].second.Position.Y = 0.5f * k; }
    P comp;
    __make_heap(a, a + 6, comp, (T*)0, (int*)0);
    if (g_adjusts != 3) { std::printf("00a32c20_TEST FAIL\n"); return 1; }   // parents 2, 1, 0
    for (int k = 1; k < 6; ++k)
        if (a[(k - 1) / 2].first < a[k].first) { std::printf("00a32c20_TEST FAIL\n"); return 1; }
    if (a[0].first != 0xFFFFFFF0UL || a[0].second.BoneIndex != 4) { std::printf("00a32c20_TEST FAIL\n"); return 1; }
    long seen = 0;
    for (int k = 0; k < 6; ++k) {
        long b = a[k].second.BoneIndex;
        if (keys[b] != a[k].first || a[k].second.Position.Y != 0.5f * b) { std::printf("00a32c20_TEST FAIL\n"); return 1; }
        seen |= 1L << b;
    }
    if (seen != 0x3f) { std::printf("00a32c20_TEST FAIL\n"); return 1; }
    std::printf("00a32c20_TEST PASS\n");
    return 0;
}