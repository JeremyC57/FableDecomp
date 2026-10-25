// Behaviour test for 0x00817540 std::__unguarded_insertion_sort_aux over THitLocationIntersectionElement by key (unguarded: a smaller element sits before first).
#include <cstdio>

struct CHitLocation { int id; };
struct C3DVector {
    float X, Y, Z;
    C3DVector() : X(0), Y(0), Z(0) { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};
struct THitLocationIntersectionElement {
    long          key;
    CHitLocation* hit_location;
    C3DVector     intersection_pos;
    C3DVector     outward_normal;
};
struct THitLocationIntersectionElementPred {
    bool operator()(const THitLocationIntersectionElement& a, const THitLocationIntersectionElement& b) const { return a.key < b.key; }
};
static void __unguarded_linear_insert(THitLocationIntersectionElement* last, THitLocationIntersectionElement val, THitLocationIntersectionElementPred comp)
{
    THitLocationIntersectionElement* next = last;
    --next;
    while (comp(val, *next)) {
        *last = *next;
        last = next;
        --next;
    }
    *last = val;
}
static void __unguarded_insertion_sort_aux(THitLocationIntersectionElement* first, THitLocationIntersectionElement* last,
                                           THitLocationIntersectionElement*, THitLocationIntersectionElementPred comp)
{
    for (THitLocationIntersectionElement* i = first; i != last; ++i)
        __unguarded_linear_insert(i, THitLocationIntersectionElement(*i), comp);
}

int main()
{
    CHitLocation hits[7];
    THitLocationIntersectionElement a[7];
    long keys[7] = { -100, 30, 10, 20, 10, -5, 40 };   // a[0] is the guard below every key
    for (int k = 0; k < 7; ++k) {
        hits[k].id = k;
        a[k].key = keys[k];
        a[k].hit_location = &hits[k];
        a[k].intersection_pos.X = 1.0f * k; a[k].intersection_pos.Y = 2.0f * k; a[k].intersection_pos.Z = 3.0f * k;
        a[k].outward_normal.X = -1.0f * k; a[k].outward_normal.Y = 0.5f * k; a[k].outward_normal.Z = 0.25f * k;
    }
    THitLocationIntersectionElementPred comp;
    __unguarded_insertion_sort_aux(a + 1, a + 7, (THitLocationIntersectionElement*)0, comp);
    // stable ascending: -100(0) -5(5) 10(2) 10(4) 20(3) 30(1) 40(6)
    int order[7] = { 0, 5, 2, 4, 3, 1, 6 };
    for (int j = 0; j < 7; ++j) {
        int k = order[j];
        if (a[j].key != keys[k] || a[j].hit_location != &hits[k] || a[j].intersection_pos.Z != 3.0f * k || a[j].outward_normal.Y != 0.5f * k)
            { std::printf("00817540_TEST FAIL\n"); return 1; }
    }
    std::printf("00817540_TEST PASS\n");
    return 0;
}