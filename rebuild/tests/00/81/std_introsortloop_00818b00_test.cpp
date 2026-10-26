// Behaviour test for 0x00818B00 __introsort_loop<THitLocationIntersectionElement> (model copy): after the loop every
// 16-element-or-smaller block is partitioned relative to its neighbours, so a final insertion sort finishes the sort;
// depth 0 forces the partial_sort fallback.
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
static const E& median_model(const E& a, const E& b, const E& c, Pred comp)
{
    if (comp(a, b)) { if (comp(b, c)) return b; else if (comp(a, c)) return c; else return a; }
    else if (comp(a, c)) return a;
    else if (comp(b, c)) return c;
    else return b;
}
static E* partition_model(E* first, E* last, E pivot, Pred comp)
{
    for (;;) {
        while (comp(*first, pivot)) ++first;
        --last;
        while (comp(pivot, *last)) --last;
        if (!(first < last)) return first;
        E t = *first; *first = *last; *last = t;
        ++first;
    }
}
static int g_partial_calls = 0;
static void partial_sort_model(E* first, E* middle, E* last, E*, Pred comp)   // full sort of [first, last) (middle == last)
{
    ++g_partial_calls;
    for (E* i = first + 1; i < middle; ++i) {
        E v = *i; E* j = i;
        while (j > first && comp(v, *(j - 1))) { *j = *(j - 1); --j; }
        *j = v;
    }
    (void)last;
}
static void introsort_loop_model(E* first, E* last, E*, int depth_limit, Pred comp)
{
    while (last - first > 16) {
        if (depth_limit == 0) { partial_sort_model(first, last, last, (E*)0, comp); return; }
        --depth_limit;
        E* cut = partition_model(first, last, E(median_model(*first, *(first + (last - first) / 2), *(last - 1), comp)), comp);
        introsort_loop_model(cut, last, (E*)0, depth_limit, comp);
        last = cut;
    }
}
static int run(int depth)
{
    E v[100];
    unsigned int seed = 12345u;
    int i;
    for (i = 0; i < 100; ++i) { seed = seed * 1103515245u + 12345u; v[i].key = (long)((seed >> 16) % 61) - 30; v[i].hit_location = 0; v[i].intersection_pos.Z = (float)v[i].key; }
    long sum0 = 0; for (i = 0; i < 100; ++i) sum0 += v[i].key;
    introsort_loop_model(v, v + 100, (E*)0, depth, Pred());
    for (i = 1; i < 100; ++i) {                     // final insertion sort
        E x = v[i]; int j = i;
        while (j > 0 && x.key < v[j - 1].key) { v[j] = v[j - 1]; --j; }
        v[j] = x;
    }
    long sum1 = 0; for (i = 0; i < 100; ++i) sum1 += v[i].key;
    if (sum0 != sum1) return 0;
    for (i = 1; i < 100; ++i) if (v[i - 1].key > v[i].key) return 0;
    for (i = 0; i < 100; ++i) if (v[i].intersection_pos.Z != (float)v[i].key) return 0;
    return 1;
}
int main()
{
    int ok = run(14);
    int before = g_partial_calls;
    ok = ok && run(0) && g_partial_calls == before + 1;
    if (!ok) { std::printf("00818b00_TEST FAIL\n"); return 1; }
    std::printf("00818b00_TEST PASS\n");
    return 0;
}