// Behaviour test for 0x009327F0 __introsort_loop: partitions into blocks of at most 16 that the final insertion sort completes; depth 0 sorts outright.
#include <cstdio>
#include <cstring>

class CThing {                              // only the position height matters to the compare
public:
    float Z;
};
typedef bool (*ThingCompare)(CThing* a, CThing* b);
static int g_compares = 0;
static bool LowerFirst(CThing* a, CThing* b) { ++g_compares; return a->Z < b->Z; }   // the 0x00930D90 compare

template <class It, class T, class Cmp>
void UnguardedLinearInsert(It last, T val, Cmp comp)
{
    It next = last;
    --next;
    while (comp(val, *next)) {
        *last = *next;
        last = next;
        --next;
    }
    *last = val;
}

template <class It, class Cmp>
void InsertionSort(It first, It last, Cmp comp)
{
    if (first == last) return;
    for (It i = first + 1; i != last; ++i) {
        CThing* val = *i;
        if (comp(val, *first)) {
            int n = (int)((char*)i - (char*)first);
            if (n > 0) std::memmove((char*)(i + 1) - n, first, n);
            *first = val;
        }
        else
            UnguardedLinearInsert(i, val, comp);
    }
}

template <class It, class Dist, class T, class Cmp>
void PushHeap(It first, Dist hole, Dist top, T val, Cmp comp)
{
    Dist parent = (hole - 1) / 2;
    while (hole > top && comp(*(first + parent), val)) {
        *(first + hole) = *(first + parent);
        hole = parent;
        parent = (hole - 1) / 2;
    }
    *(first + hole) = val;
}

template <class It, class Dist, class T, class Cmp>
void AdjustHeap(It first, Dist hole, Dist len, T val, Cmp comp)
{
    Dist top = hole;
    Dist second = 2 * hole + 2;
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
    PushHeap(first, hole, top, val, comp);
}

template <class It, class Cmp>
void MakeHeap(It first, It last, Cmp comp)
{
    if (last - first < 2) return;
    int len = (int)(last - first);
    int parent = (len - 2) / 2;
    for (;;) {
        AdjustHeap(first, parent, len, *(first + parent), comp);
        if (parent == 0) return;
        parent--;
    }
}

template <class It, class Cmp>
void SortHeap(It first, It last, Cmp comp)
{
    while (last - first > 1) {
        CThing* val = *(last - 1);
        *(last - 1) = *first;
        AdjustHeap(first, 0, (int)((last - 1) - first), val, comp);
        --last;
    }
}

template <class It, class Cmp>
void PartialSort(It first, It middle, It last, Cmp comp)
{
    MakeHeap(first, middle, comp);
    for (It i = middle; i < last; ++i)
        if (comp(*i, *first)) {
            CThing* val = *i;
            *i = *first;
            AdjustHeap(first, 0, (int)(middle - first), val, comp);
        }
    SortHeap(first, middle, comp);
}

template <class T, class Cmp>
const T& Median(const T& a, const T& b, const T& c, Cmp comp)
{
    if (comp(a, b)) {
        if (comp(b, c)) return b;
        else if (comp(a, c)) return c;
        else return a;
    }
    else if (comp(a, c)) return a;
    else if (comp(b, c)) return c;
    else return b;
}

template <class It, class T, class Cmp>
It UnguardedPartition(It first, It last, T pivot, Cmp comp)
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

template <class It, class Cmp>
void IntrosortLoop(It first, It last, int depth_limit, Cmp comp)
{
    It cut;
    while (last - first > 16) {
        if (depth_limit == 0) {
            PartialSort(first, last, last, comp);
            return;
        }
        --depth_limit;
        cut = UnguardedPartition(first, last, Median(*first, *(first + (last - first) / 2), *(last - 1), comp), comp);
        IntrosortLoop(cut, last, depth_limit, comp);
        last = cut;
    }
}

static CThing g_things[64];
static CThing* g_ptrs[64];
static void Fill(int n, int seed)
{
    unsigned int s = (unsigned int)seed;
    for (int i = 0; i < n; ++i) {
        s = s * 1103515245u + 12345u;
        g_things[i].Z = (float)((s >> 16) % 50u) * 0.5f;
        g_ptrs[i] = &g_things[i];
    }
}
static bool Sorted(CThing** p, int n)
{
    for (int i = 1; i < n; ++i)
        if (p[i]->Z < p[i - 1]->Z) return false;
    return true;
}
static bool SamePointers(CThing** p, int n)        // a permutation of g_things[0..n)
{
    int seen[64];
    for (int i = 0; i < n; ++i) seen[i] = 0;
    for (int i = 0; i < n; ++i) {
        int k = (int)(p[i] - g_things);
        if (k < 0 || k >= n || seen[k]) return false;
        seen[k] = 1;
    }
    return true;
}

int main()
{
    int ok = 1;
    for (int depth = 0; depth <= 10; depth += 5) {
        Fill(60, depth + 7);
        IntrosortLoop(g_ptrs, g_ptrs + 60, depth, (ThingCompare)LowerFirst);
        if (depth == 0 && !Sorted(g_ptrs, 60)) ok = 0;
        InsertionSort(g_ptrs, g_ptrs + 60, (ThingCompare)LowerFirst);
        if (!Sorted(g_ptrs, 60) || !SamePointers(g_ptrs, 60)) ok = 0;
    }
    Fill(10, 3);
    g_compares = 0;
    IntrosortLoop(g_ptrs, g_ptrs + 10, 4, (ThingCompare)LowerFirst);          // short range: untouched
    if (g_compares != 0) ok = 0;
    if (!ok) { std::printf("009327f0_TEST FAIL\n"); return 1; }
    std::printf("009327f0_TEST PASS\n");
    return 0;
}