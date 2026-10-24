// Behaviour test for 0x00818450 __partial_sort<THitLocationIntersectionElement> (model copy): the smallest
// (middle - first) keys end up sorted at the front.
#include <cstdio>
struct CHitLocation { int id; };
struct C3DVector {
    float X, Y, Z;
    C3DVector() : X(0), Y(0), Z(0) {}
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};
struct E {
    long          key;
    CHitLocation* hit_location;
    C3DVector     intersection_pos;
    C3DVector     outward_normal;
};
struct Pred { bool operator()(const E& a, const E& b) const { return a.key < b.key; } };
static void push_heap_model(E* first, int holeIndex, int topIndex, E val, Pred comp)
{
    int parent = (holeIndex - 1) / 2;
    while (holeIndex > topIndex && comp(first[parent], val)) { first[holeIndex] = first[parent]; holeIndex = parent; parent = (holeIndex - 1) / 2; }
    first[holeIndex] = val;
}
static void adjust_heap_model(E* first, int holeIndex, int len, E val, Pred comp)
{
    int topIndex = holeIndex;
    int secondChild = 2 * holeIndex + 2;
    while (secondChild < len) {
        if (comp(first[secondChild], first[secondChild - 1])) secondChild--;
        first[holeIndex] = first[secondChild];
        holeIndex = secondChild;
        secondChild = 2 * (secondChild + 1);
    }
    if (secondChild == len) { first[holeIndex] = first[secondChild - 1]; holeIndex = secondChild - 1; }
    push_heap_model(first, holeIndex, topIndex, val, comp);
}
static void make_heap_model(E* first, E* last, Pred comp)
{
    if (last - first < 2) return;
    int len = (int)(last - first);
    int parent = (len - 2) / 2;
    for (;;) { adjust_heap_model(first, parent, len, E(first[parent]), comp); if (parent == 0) return; parent--; }
}
static void sort_heap_model(E* first, E* last, Pred comp)
{
    while (last - first > 1) { E v = *(last - 1); *(last - 1) = *first; --last; adjust_heap_model(first, 0, (int)(last - first), v, comp); }
}
static void partial_sort_model(E* first, E* middle, E* last, E*, Pred comp)
{
    make_heap_model(first, middle, comp);
    for (E* i = middle; i < last; ++i)
        if (comp(*i, *first)) { E v(*i); *i = *first; adjust_heap_model(first, 0, (int)(middle - first), v, comp); }
    sort_heap_model(first, middle, comp);
}
int main()
{
    static const long keys[10] = { 7, 3, 9, -4, 12, 0, 5, 3, 8, 1 };
    E v[10];
    int i;
    for (i = 0; i < 10; ++i) { v[i].key = keys[i]; v[i].hit_location = 0; v[i].outward_normal.X = (float)keys[i]; }
    partial_sort_model(v, v + 4, v + 10, (E*)0, Pred());
    static const long want[4] = { -4, 0, 1, 3 };
    int ok = 1;
    for (i = 0; i < 4; ++i) if (v[i].key != want[i] || v[i].outward_normal.X != (float)want[i]) ok = 0;
    long sum = 0; for (i = 0; i < 10; ++i) sum += v[i].key;
    if (sum != 44) ok = 0;
    if (!ok) { std::printf("00818450_TEST FAIL\n"); return 1; }
    std::printf("00818450_TEST PASS\n");
    return 0;
}