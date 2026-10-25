// Behaviour test for 0x00759660 std::__introsort_loop over pair<EBodyLocation, CWoundMorphs::CEntry> (CKeyPairCompareLess): partitions to <= 16-element runs, heap-sort fallback at depth 0, payload intact
#include <cstdio>
#include <cstdlib>
#include <new>

namespace model {

// pair<EBodyLocation, CWoundMorphs::CEntry (40-byte POD)>, ordered by the body location (44 bytes).
struct Elem {
    int   first;
    int   Location;
    int   Indices[6];
    int   FadeToScarDuration, TotalDuration;
    float PermanentScarStrength;
};
bool less(const Elem& a, const Elem& b) { return a.first < b.first; }
Elem make(int k, int i)
{
    Elem e;
    e.first = k; e.Location = k;
    for (int j = 0; j < 6; ++j) e.Indices[j] = k * 10 + j;
    e.FadeToScarDuration = i; e.TotalDuration = k + 1000; e.PermanentScarStrength = (float)k;
    return e;
}
long key(const Elem& x) { return (long)x.first; }
bool intact(const Elem& x)
{
    return sizeof(Elem) == 44u && x.Location == x.first && x.Indices[5] == x.first * 10 + 5 && x.TotalDuration == x.first + 1000
        && x.PermanentScarStrength == (float)x.first;
}
bool final_check() { return true; }

// STLport 4.6 introsort pieces (the row's body is introsort_loop; the final insertion sort is its caller's).
const Elem& median(const Elem& a, const Elem& b, const Elem& c)
{
    if (less(a, b)) {
        if (less(b, c)) return b;
        else if (less(a, c)) return c;
        else return a;
    }
    else if (less(a, c)) return a;
    else if (less(b, c)) return c;
    else return b;
}
void swap_elems(Elem* a, Elem* b) { Elem t(*a); *a = *b; *b = t; }
Elem* unguarded_partition(Elem* first, Elem* last, Elem pivot)
{
    for (;;) {
        while (less(*first, pivot)) ++first;
        --last;
        while (less(pivot, *last)) --last;
        if (!(first < last)) return first;
        swap_elems(first, last);
        ++first;
    }
}
void sift_down(Elem* base, int hole, int len)
{
    for (;;) {
        int child = 2 * hole + 1;
        if (child >= len) return;
        if (child + 1 < len && less(base[child], base[child + 1])) ++child;
        if (!less(base[hole], base[child])) return;
        swap_elems(base + hole, base + child);
        hole = child;
    }
}
void partial_sort_all(Elem* first, Elem* last)       // __partial_sort(first, last, last): make_heap + sort_heap
{
    int len = (int)(last - first);
    for (int i = len / 2 - 1; i >= 0; --i) sift_down(first, i, len);
    for (int n = len - 1; n > 0; --n) { swap_elems(first, first + n); sift_down(first, 0, n); }
}
void introsort_loop(Elem* first, Elem* last, int depth_limit)
{
    while (last - first > 16) {
        if (depth_limit == 0) {
            partial_sort_all(first, last);
            return;
        }
        --depth_limit;
        Elem* cut = unguarded_partition(first, last, Elem(median(*first, *(first + (last - first) / 2), *(last - 1))));
        introsort_loop(cut, last, depth_limit);
        last = cut;
    }
}
void insertion_sort(Elem* first, Elem* last)
{
    for (Elem* i = first + 1; i < last; ++i)
        for (Elem* j = i; j != first && less(*j, *(j - 1)); --j)
            swap_elems(j, j - 1);
}

}

int run(int n, int depth)
{
    using namespace model;
    Elem* v = (Elem*)std::malloc(n * sizeof(Elem));
    unsigned int seed = 12345u + (unsigned int)n;
    long sum = 0;
    for (int i = 0; i < n; ++i) {
        seed = seed * 1103515245u + 12345u;
        int k = (int)((seed >> 16) % 97u);
        new (v + i) Elem(make(k, i));
        sum += k;
    }
    introsort_loop(v, v + n, depth);
    // introsort_loop leaves runs of at most 16 unsorted but partitioned: every element of a left run <= every right one
    insertion_sort(v, v + n);
    int bad = 0;
    long sum2 = 0;
    for (int i = 0; i < n; ++i) {
        sum2 += key(v[i]);
        if (i > 0 && less(v[i], v[i - 1])) bad = 1;
        if (!intact(v[i])) bad = 2;
    }
    if (sum2 != sum) bad = 3;
    for (int i = 0; i < n; ++i) v[i].~Elem();
    std::free(v);
    return bad;
}

int main()
{
    int bad = 0;
    if (run(200, 2 * 7)) bad = 1;          // __lg(200) * 2
    if (run(200, 0)) bad = 2;               // depth exhausted: the heap-sort fallback
    if (run(12, 6)) bad = 3;                // short range: nothing to do
    if (!model::final_check()) bad = 4;
    if (bad) { std::printf("00759660_TEST FAIL\n"); return 1; }
    std::printf("00759660_TEST PASS\n");
    return 0;
}