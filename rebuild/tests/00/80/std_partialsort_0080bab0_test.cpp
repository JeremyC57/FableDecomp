// Behaviour test for 0x0080BAB0 std::__partial_sort over NPlayerTargeting::CTargetPoint with CThingCompare_BestForTargeting2
#include <cstdio>

// STLport 4.6 heap / sort model shared by the generated tests (plain C++98).
template <class RI, class T, class Cmp>
void tm_push_heap(RI first, int hole, int top, T val, Cmp comp)
{
    int parent = (hole - 1) / 2;
    while (hole > top && comp(*(first + parent), val)) {
        *(first + hole) = *(first + parent);
        hole = parent;
        parent = (hole - 1) / 2;
    }
    *(first + hole) = val;
}
template <class RI, class T, class Cmp>
void tm_adjust_heap(RI first, int hole, int len, T val, Cmp comp)
{
    int top = hole;
    int second = 2 * hole + 2;
    while (second < len) {
        if (comp(*(first + second), *(first + (second - 1))))
            second--;
        *(first + hole) = *(first + second);
        hole = second;
        second = 2 * (second + 1);
    }
    if (second == len) {
        *(first + hole) = *(first + (second - 1));
        hole = second - 1;
    }
    tm_push_heap(first, hole, top, val, comp);
}
template <class RI, class T, class Cmp>
void tm_make_heap(RI first, RI last, T*, Cmp comp)
{
    if (last - first < 2) return;
    int len = last - first;
    int parent = (len - 2) / 2;
    for (;;) {
        tm_adjust_heap(first, parent, len, T(*(first + parent)), comp);
        if (parent == 0) return;
        parent--;
    }
}
template <class RI, class T, class Cmp>
void tm_pop_heap(RI first, RI last, RI result, T val, Cmp comp)
{
    *result = *first;
    tm_adjust_heap(first, 0, last - first, val, comp);
}
template <class RI, class T, class Cmp>
void tm_sort_heap(RI first, RI last, T*, Cmp comp)
{
    while (last - first > 1) {
        T v = *(last - 1);
        tm_pop_heap(first, last - 1, last - 1, v, comp);
        --last;
    }
}
template <class RI, class T, class Cmp>
void tm_partial_sort(RI first, RI middle, RI last, T* tp, Cmp comp)
{
    tm_make_heap(first, middle, tp, comp);
    for (RI i = middle; i < last; ++i)
        if (comp(*i, *first))
            tm_pop_heap(first, middle, i, T(*i), comp);
    tm_sort_heap(first, middle, tp, comp);
}
template <class T, class Cmp>
const T& tm_median(const T& a, const T& b, const T& c, Cmp comp)
{
    if (comp(a, b))
        if (comp(b, c)) return b;
        else if (comp(a, c)) return c;
        else return a;
    else if (comp(a, c)) return a;
    else if (comp(b, c)) return c;
    else return b;
}
template <class RI, class T, class Cmp>
RI tm_unguarded_partition(RI first, RI last, T pivot, Cmp comp)
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
template <class RI, class T, class Cmp>
void tm_introsort_loop(RI first, RI last, T* tp, int depth, Cmp comp)
{
    while (last - first > 16) {
        if (depth == 0) {
            tm_partial_sort(first, last, last, tp, comp);
            return;
        }
        --depth;
        RI cut = tm_unguarded_partition(first, last, T(tm_median(*first, *(first + (last - first) / 2), *(last - 1), comp)), comp);
        tm_introsort_loop(cut, last, tp, depth, comp);
        last = cut;
    }
}
template <class RI, class T, class Cmp>
void tm_unguarded_linear_insert(RI last, T val, Cmp comp)
{
    RI next = last;
    --next;
    while (comp(val, *next)) {
        *last = *next;
        last = next;
        --next;
    }
    *last = val;
}
template <class RI, class T, class Cmp>
void tm_insertion_sort(RI first, RI last, T*, Cmp comp)
{
    if (first == last) return;
    for (RI i = first + 1; i != last; ++i) {
        T val = *i;
        if (comp(val, *first)) {
            for (RI j = i; j != first; --j) *j = *(j - 1);
            *first = val;
        } else
            tm_unguarded_linear_insert(i, val, comp);
    }
}

static unsigned int g_rng = 12345u;
static int next_rand(int n) { g_rng = g_rng * 1103515245u + 12345u; return (int)((g_rng >> 8) % (unsigned int)n); }

struct Elem {                    // stands in for the element type (score = what the comparator ranks)
    int score;
    int id;
};
struct BestFirst {               // the targeting comparators: true when a scores above b (best first)
    int calls;
    bool operator()(const Elem& a, const Elem& b) const { return a.score > b.score; }
};
static void fill(Elem* v, int n) { for (int i = 0; i < n; ++i) { v[i].score = next_rand(50); v[i].id = i; } }
static bool is_perm(const Elem* v, int n)
{
    int seen[200];
    int i;
    for (i = 0; i < n; ++i) seen[i] = 0;
    for (i = 0; i < n; ++i) { if (v[i].id < 0 || v[i].id >= n || seen[v[i].id]) return false; seen[v[i].id] = 1; }
    return true;
}
static void ref_sort(Elem* v, int n)   // stable selection by score, descending
{
    for (int i = 0; i < n; ++i)
        for (int j = i + 1; j < n; ++j)
            if (v[j].score > v[i].score) { Elem t = v[i]; v[i] = v[j]; v[j] = t; }
}

int main()
{
    BestFirst comp; comp.calls = 0;
    for (int round = 0; round < 200; ++round) {
        Elem v[120], r[120];
        int n = 1 + next_rand(119);
        int k = next_rand(n + 1);
        fill(v, n);
        int i;
        for (i = 0; i < n; ++i) r[i] = v[i];
        ref_sort(r, n);
        tm_partial_sort(v, v + k, v + n, (Elem*)0, comp);
        if (!is_perm(v, n)) { std::printf("0080bab0_TEST FAIL\n"); return 1; }
        for (i = 0; i < k; ++i)
            if (v[i].score != r[i].score) { std::printf("0080bab0_TEST FAIL\n"); return 1; }
    }
    std::printf("0080bab0_TEST PASS\n");
    return 0;
}