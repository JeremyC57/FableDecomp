// Behaviour test for 0x00818240 __linear_insert<THitLocationIntersectionElement> (model copy) driving an insertion sort.
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
static void unguarded_linear_insert_model(E* last, E val, Pred comp)
{
    E* next = last;
    --next;
    while (comp(val, *next)) { *last = *next; last = next; --next; }
    *last = val;
}
static E* copy_backward_model(E* first, E* last, E* result)
{
    for (int n = (int)(last - first); n > 0; --n) *--result = *--last;
    return result;
}
static void linear_insert_model(E* first, E* last, E val, Pred comp)
{
    if (comp(val, *first)) {
        copy_backward_model(first, last, last + 1);
        *first = val;
    }
    else
        unguarded_linear_insert_model(last, val, comp);
}
int main()
{
    static const long keys[9] = { 4, 8, -2, 4, 1, 9, 0, 3, -2 };
    E v[9];
    int i;
    for (i = 0; i < 9; ++i) { v[i].key = keys[i]; v[i].hit_location = 0; v[i].intersection_pos.Y = (float)(keys[i] * 2); }
    for (i = 1; i < 9; ++i) linear_insert_model(v, v + i, v[i], Pred());
    static const long want[9] = { -2, -2, 0, 1, 3, 4, 4, 8, 9 };
    int ok = 1;
    for (i = 0; i < 9; ++i) if (v[i].key != want[i] || v[i].intersection_pos.Y != (float)(want[i] * 2)) ok = 0;
    if (!ok) { std::printf("00818240_TEST FAIL\n"); return 1; }
    std::printf("00818240_TEST PASS\n");
    return 0;
}