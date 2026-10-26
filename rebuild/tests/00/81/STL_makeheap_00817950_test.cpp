// Behaviour test for 0x00817950 std::__make_heap over THitLocationIntersectionElement by key (the SGI __adjust_heap / __push_heap pair stands in for 0x008175D0).
#include <cstdio>

struct CHitLocation { int id; };
class C3DVector { public:
    float X, Y, Z;
    C3DVector() : X(0), Y(0), Z(0) { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};
struct THitLocationIntersectionElement { long key; CHitLocation* hit_location; C3DVector intersection_pos; C3DVector outward_normal; };
struct THitLocationIntersectionElementPred {
    bool operator()(const THitLocationIntersectionElement& a, const THitLocationIntersectionElement& b) const { return a.key < b.key; }
};
typedef THitLocationIntersectionElement T;
typedef THitLocationIntersectionElementPred P;

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
    CHitLocation hits[9];
    T a[9];
    long keys[9] = { 4, 17, -2, 9, 30, 1, 30, 8, 12 };
    for (int k = 0; k < 9; ++k) { hits[k].id = k; a[k].key = keys[k]; a[k].hit_location = &hits[k]; a[k].intersection_pos.X = (float)keys[k]; }
    P comp;
    __make_heap(a, a + 9, comp, (T*)0, (int*)0);
    if (g_adjusts != 4) { std::printf("00817950_TEST FAIL\n"); return 1; }   // parents 3, 2, 1, 0
    for (int k = 1; k < 9; ++k)
        if (a[(k - 1) / 2].key < a[k].key) { std::printf("00817950_TEST FAIL\n"); return 1; }
    long sum = 0;
    for (int k = 0; k < 9; ++k) {
        if (a[k].hit_location->id < 0 || keys[a[k].hit_location->id] != a[k].key || a[k].intersection_pos.X != (float)a[k].key)
            { std::printf("00817950_TEST FAIL\n"); return 1; }
        sum += a[k].hit_location->id;
    }
    if (sum != 36 || a[0].key != 30) { std::printf("00817950_TEST FAIL\n"); return 1; }
    __make_heap(a, a + 1, comp, (T*)0, (int*)0);   // fewer than two elements: nothing to do
    if (g_adjusts != 4) { std::printf("00817950_TEST FAIL\n"); return 1; }
    std::printf("00817950_TEST PASS\n");
    return 0;
}