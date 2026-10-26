// Behaviour test for 0x00817840 __unguarded_partition<THitLocationIntersectionElement> (model copy).
#include <cstdio>
struct CHitLocation { int id; };
class C3DVector { public:
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
template <class T> inline void swap_model(T& a, T& b) { T tmp = a; a = b; b = tmp; }
static E* partition_model(E* first, E* last, E pivot, Pred comp)
{
    for (;;) {
        while (comp(*first, pivot)) ++first;
        --last;
        while (comp(pivot, *last)) --last;
        if (!(first < last)) return first;
        swap_model(*first, *last);
        ++first;
    }
}
int main()
{
    static const long keys[8] = { 6, 2, 9, 5, 1, 8, 3, 7 };
    E v[8];
    int i;
    for (i = 0; i < 8; ++i) { v[i].key = keys[i]; v[i].hit_location = 0; v[i].outward_normal.Z = (float)keys[i]; }
    E pivot = v[3];                      // key 5 (median-ish, present -> guards both scans)
    E* cut = partition_model(v, v + 8, pivot, Pred());
    int k = (int)(cut - v);
    int ok = (k > 0 && k < 8);
    for (i = 0; i < k; ++i) if (v[i].key > 5) ok = 0;
    for (i = k; i < 8; ++i) if (v[i].key < 5) ok = 0;
    for (i = 0; i < 8; ++i) if (v[i].outward_normal.Z != (float)v[i].key) ok = 0;
    long sum = 0; for (i = 0; i < 8; ++i) sum += v[i].key;
    if (sum != 41) ok = 0;
    if (!ok) { std::printf("00817840_TEST FAIL\n"); return 1; }
    std::printf("00817840_TEST PASS\n");
    return 0;
}