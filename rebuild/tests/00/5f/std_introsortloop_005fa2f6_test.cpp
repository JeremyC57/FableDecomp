// Behaviour test for 0x005FA2F6 std::__introsort_loop(first, last, pair<long, long>*, depth_limit, CKeyPairCompareLess):
// median-of-three quicksort down to 16-element runs, partial_sort (heap sort) once the depth budget is spent. The
// callees (__median, __unguarded_partition, partial_sort) are modelled; a final insertion sort checks the run invariant.
#include <cstdio>

enum EHeroAbility { ABILITY_NULL = 0, EHeroAbility_Force32 = 0x7fffffff };

namespace stl {
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair() : first(), second() { }
    pair(const T1& a, const T2& b) : first(a), second(b) { }
    pair(const pair& o) : first(o.first), second(o.second) { }
};
}

template <class K, class V> class CKeyPairCompareLess { public:
    bool operator()(const stl::pair<K, V>& a, const stl::pair<K, V>& b) const { return a.first < b.first; }
};

namespace stl {
template <class RandomAccessIter, class Distance, class T, class Compare>
void __push_heap(RandomAccessIter first, Distance holeIndex, Distance topIndex, T val, Compare comp)
{
    Distance parent = (holeIndex - 1) / 2;
    while (holeIndex > topIndex && comp(*(first + parent), val)) {
        *(first + holeIndex) = *(first + parent);
        holeIndex = parent;
        parent = (holeIndex - 1) / 2;
    }
    *(first + holeIndex) = val;
}
template <class RandomAccessIter, class Distance, class T, class Compare>
void __adjust_heap(RandomAccessIter first, Distance holeIndex, Distance len, T val, Compare comp)
{
    Distance topIndex = holeIndex;
    Distance secondChild = 2 * holeIndex + 2;
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
    __push_heap(first, holeIndex, topIndex, val, comp);
}
template <class RandomAccessIter, class T, class Compare, class Distance>
void __pop_heap(RandomAccessIter first, RandomAccessIter last, RandomAccessIter result, T value, Compare comp, Distance*)
{
    *result = *first;
    __adjust_heap(first, Distance(0), Distance(last - first), value, comp);
}
template <class RandomAccessIter, class Compare>
void make_heap(RandomAccessIter first, RandomAccessIter last, Compare comp)
{
    int len = int(last - first);
    if (len < 2)
        return;
    int parent = (len - 2) / 2;
    for (;;) {
        __adjust_heap(first, parent, len, *(first + parent), comp);
        if (parent == 0)
            return;
        parent--;
    }
}
template <class RandomAccessIter, class Compare>
void sort_heap(RandomAccessIter first, RandomAccessIter last, Compare comp)
{
    while (last - first > 1) {
        --last;
        __pop_heap(first, last, last, *last, comp, (int*)0);
    }
}
}

namespace stl {
const int __stl_threshold = 16;

template <class T, class Compare>
const T& __median(const T& a, const T& b, const T& c, Compare comp)
{
    if (comp(a, b))
        if (comp(b, c)) return b;
        else if (comp(a, c)) return c;
        else return a;
    else if (comp(a, c)) return a;
    else if (comp(b, c)) return c;
    else return b;
}
template <class RandomAccessIter, class T, class Compare>
RandomAccessIter __unguarded_partition(RandomAccessIter first, RandomAccessIter last, T pivot, Compare comp)
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
template <class RandomAccessIter, class Compare>
void partial_sort(RandomAccessIter first, RandomAccessIter middle, RandomAccessIter last, Compare comp)
{
    make_heap(first, middle, comp);
    for (RandomAccessIter i = middle; i < last; ++i)
        if (comp(*i, *first))
            __pop_heap(first, middle, i, *i, comp, (int*)0);
    sort_heap(first, middle, comp);
}

template <class RandomAccessIter, class T, class Size, class Compare>
void __introsort_loop(RandomAccessIter first, RandomAccessIter last, T*, Size depth_limit, Compare comp)
{
    while (last - first > __stl_threshold) {
        if (depth_limit == 0) {
            partial_sort(first, last, last, comp);
            return;
        }
        --depth_limit;
        RandomAccessIter cut =
            __unguarded_partition(first, last, T(__median(*first, *(first + (last - first) / 2), *(last - 1), comp)), comp);
        __introsort_loop(cut, last, (T*)0, depth_limit, comp);
        last = cut;
    }
}
}

typedef stl::pair<long, long> P;

static void Fill(P* v, int n)
{
    unsigned long seed = 12345;
    for (int i = 0; i < n; ++i) {
        seed = seed * 1103515245UL + 12345UL;
        long k = (long)((seed >> 8) % 1000);
        v[i] = P(k, (long)(k + 7));
    }
}

static int Check(P* v, int n, long sum)
{
    int ok = 1;
    long s = 0;
    for (int i = 0; i < n; ++i) {
        s += v[i].first;
        if (v[i].second != (long)(v[i].first + 7)) ok = 0;
    }
    return ok && s == sum;
}

int main()
{
    int ok = 1;
    const int N = 100;
    P v[N];
    Fill(v, N);
    long sum = 0;
    for (int i = 0; i < N; ++i) sum += v[i].first;
    CKeyPairCompareLess<long, long> cmp;
    // depth budget spent at once: the whole range is heap sorted
    stl::__introsort_loop(v, v + N, (P*)0, 0, cmp);
    for (int i = 1; i < N; ++i)
        if (v[i].first < v[i - 1].first) ok = 0;
    if (!Check(v, N, sum)) ok = 0;
    // normal budget: afterwards every element is within its 16-element run (insertion sort moves nothing far)
    Fill(v, N);
    stl::__introsort_loop(v, v + N, (P*)0, 14, cmp);
    if (!Check(v, N, sum)) ok = 0;
    int maxShift = 0;
    for (int i = 1; i < N; ++i) {
        P val = v[i];
        int j = i;
        while (j > 0 && val.first < v[j - 1].first) { v[j] = v[j - 1]; --j; }
        v[j] = val;
        if (i - j > maxShift) maxShift = i - j;
    }
    for (int i = 1; i < N; ++i)
        if (v[i].first < v[i - 1].first) ok = 0;
    if (maxShift > 16) ok = 0;
    if (!ok) { std::printf("005fa2f6_TEST FAIL\n"); return 1; }
    std::printf("005fa2f6_TEST PASS\n");
    return 0;
}