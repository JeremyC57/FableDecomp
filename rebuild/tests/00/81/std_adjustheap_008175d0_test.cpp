// Behaviour test for 0x008175D0 __adjust_heap<THitLocationIntersectionElement> (model copy): sift-down + push_heap.
#include <cstdio>
struct CHitLocation { int id; };
class C3DVector { public:
    float X, Y, Z;
    C3DVector() : X(0), Y(0), Z(0) {}
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};
struct THitLocationIntersectionElement {
    long          key;
    CHitLocation* hit_location;
    C3DVector     intersection_pos;
    C3DVector     outward_normal;
};
struct Pred {
    bool operator()(const THitLocationIntersectionElement& a, const THitLocationIntersectionElement& b) const { return a.key < b.key; }
};
typedef THitLocationIntersectionElement E;
static void push_heap_model(E* first, int holeIndex, int topIndex, E val, Pred comp)
{
    int parent = (holeIndex - 1) / 2;
    while (holeIndex > topIndex && comp(first[parent], val)) {
        first[holeIndex] = first[parent];
        holeIndex = parent;
        parent = (holeIndex - 1) / 2;
    }
    first[holeIndex] = val;
}
static void adjust_heap_model(E* first, int holeIndex, int len, E val, Pred comp)
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
    push_heap_model(first, holeIndex, topIndex, val, comp);
}
int main()
{
    static const long keys[9] = { 5, 1, 9, 3, 7, 2, 8, 6, 4 };
    E v[9];
    int i;
    for (i = 0; i < 9; ++i) { v[i].key = keys[i]; v[i].hit_location = 0; v[i].intersection_pos.X = (float)keys[i]; }
    Pred p;
    for (i = (9 - 2) / 2; i >= 0; --i) adjust_heap_model(v, i, 9, v[i], p);    // make_heap
    int ok = 1;
    for (i = 1; i < 9; ++i) if (v[(i - 1) / 2].key < v[i].key) ok = 0;
    if (v[0].key != 9) ok = 0;
    int n;
    for (n = 9; n > 1; --n) { E t = v[n - 1]; v[n - 1] = v[0]; adjust_heap_model(v, 0, n - 1, t, p); }  // sort_heap
    for (i = 0; i < 9; ++i) if (v[i].key != i + 1 || v[i].intersection_pos.X != (float)(i + 1)) ok = 0;
    if (!ok) { std::printf("008175d0_TEST FAIL\n"); return 1; }
    std::printf("008175d0_TEST PASS\n");
    return 0;
}