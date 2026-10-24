// Behaviour test for 0x00769420 STLport __unguarded_linear_insert over (key, C3DVector) pairs: val lands in key order, payload intact.
#include <cstdio>

struct C3DVector {
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};
struct Elem {
    unsigned int first;
    C3DVector second;
    Elem() : first(0), second(0.0f, 0.0f, 0.0f) { }
    Elem(unsigned int k) : first(k), second((float)k, (float)k * 2.0f, (float)k * 3.0f) { }
    Elem(const Elem& o) : first(o.first), second(o.second) { }
};
static int gDestroyed = 0;
static bool Consistent(const Elem& e) { return e.second.X == (float)e.first && e.second.Y == (float)e.first * 2.0f && e.second.Z == (float)e.first * 3.0f; }
struct Less { bool operator()(const Elem& a, const Elem& b) const { return a.first < b.first; } };

template <class I, class T, class C>
void unguarded_linear_insert(I last, T val, C comp)
{
    I next = last; --next;
    while (comp(val, *next)) { *last = *next; last = next; --next; }
    *last = val;
}
template <class I, class T, class C>
void unguarded_insertion_sort_aux(I first, I last, T*, C comp)
{
    for (I i = first; i != last; ++i) unguarded_linear_insert(i, T(*i), comp);
}
template <class I, class T, class C>
I unguarded_partition(I first, I last, T pivot, C comp)
{
    for (;;) {
        while (comp(*first, pivot)) ++first;
        --last;
        while (comp(pivot, *last)) --last;
        if (!(first < last)) return first;
        T tmp = *first; *first = *last; *last = tmp;
        ++first;
    }
}
template <class I, class D, class T, class C>
void push_heap_(I first, D hole, D top, T val, C comp)
{
    D parent = (hole - 1) / 2;
    while (hole > top && comp(*(first + parent), val)) { *(first + hole) = *(first + parent); hole = parent; parent = (hole - 1) / 2; }
    *(first + hole) = val;
}
template <class I, class D, class T, class C>
void adjust_heap(I first, D hole, D len, T val, C comp)
{
    D top = hole, child = 2 * hole + 2;
    while (child < len) {
        if (comp(*(first + child), *(first + (child - 1)))) child--;
        *(first + hole) = *(first + child); hole = child; child = 2 * (child + 1);
    }
    if (child == len) { *(first + hole) = *(first + (child - 1)); hole = child - 1; }
    push_heap_(first, hole, top, val, comp);
}
template <class I, class C, class T, class D>
void make_heap_(I first, I last, C comp, T*, D*)
{
    if (last - first < 2) return;
    D len = (D)(last - first);
    D parent = (len - 2) / 2;
    for (;;) {
        adjust_heap(first, parent, len, T(*(first + parent)), comp);
        if (parent == 0) return;
        parent--;
    }
}
template <class I, class C>
void sort_heap_(I first, I last, C comp)
{
    while (last - first > 1) { --last; Elem v(*last); *last = *first; adjust_heap(first, 0, (int)(last - first), v, comp); }
}
template <class I, class T, class C>
void partial_sort_(I first, I middle, I last, T*, C comp)
{
    make_heap_(first, middle, comp, (T*)0, (int*)0);
    for (I i = middle; i < last; ++i)
        if (comp(*i, *first)) { T v(*i); *i = *first; adjust_heap(first, 0, (int)(middle - first), v, comp); }
    sort_heap_(first, middle, comp);
}
template <class I>
I copy_backward_(I first, I last, I result) { for (int n = (int)(last - first); n > 0; --n) *--result = *--last; return result; }
template <class I, class T, class C>
void linear_insert(I first, I last, T val, C comp)
{
    if (comp(val, *first)) { copy_backward_(first, last, last + 1); *first = val; }
    else unguarded_linear_insert(last, val, comp);
}
static const unsigned int kKeys[10] = { 5u, 9u, 1u, 7u, 3u, 8u, 2u, 6u, 4u, 0u };
int main()
{
    Elem v[6];
    unsigned int init[5] = { 1u, 3u, 5u, 8u, 9u };
    for (int i = 0; i < 5; ++i) v[i] = Elem(init[i]);
    unguarded_linear_insert(v + 5, Elem(4u), Less());
    unsigned int want[6] = { 1u, 3u, 4u, 5u, 8u, 9u };
    int ok = 1;
    for (int i = 0; i < 6; ++i) if (v[i].first != want[i] || !Consistent(v[i])) ok = 0;
    unguarded_linear_insert(v + 5, Elem(10u), Less());          // already in place: no shift
    if (v[5].first != 10u || v[4].first != 8u) ok = 0;
    if (!ok) { std::printf("00769420_TEST FAIL\n"); return 1; }
    std::printf("00769420_TEST PASS\n");
    return 0;
}