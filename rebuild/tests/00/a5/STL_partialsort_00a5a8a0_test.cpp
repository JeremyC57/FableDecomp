// Behaviour test for 0x00A5A8A0 _STL::__partial_sort<float*, float, less<float> >
#include <cstdio>
#include <cstddef>

namespace _STL {
template <class T> struct less {
    bool operator()(const T& x, const T& y) const { return x < y; }
};
// stand-ins for the out-of-line heap helpers (0x00A5A660 / 0x00A5A580 / 0x00A5A7D0): STLport bodies
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
template <class RandomAccessIter, class Compare, class T, class Distance>
void __make_heap(RandomAccessIter first, RandomAccessIter last, Compare comp, T*, Distance*)
{
    if (last - first < 2) return;
    Distance len = last - first;
    Distance parent = (len - 2) / 2;
    for (;;) {
        __adjust_heap(first, parent, len, T(*(first + parent)), comp);
        if (parent == 0) return;
        parent--;
    }
}
template <class Compare>
void sort_heap(float* first, float* last, Compare comp)
{
    while (last - first > 1) {
        --last;
        float val = *last;
        *last = *first;
        __adjust_heap(first, (ptrdiff_t)0, (ptrdiff_t)(last - first), val, comp);
    }
}
template <class RandomAccessIter, class Compare>
inline void make_heap(RandomAccessIter first, RandomAccessIter last, Compare comp)
{
    __make_heap(first, last, comp, (float*)0, (ptrdiff_t*)0);
}
template <class RandomAccessIter, class T, class Compare, class Distance>
inline void __pop_heap(RandomAccessIter first, RandomAccessIter last, RandomAccessIter result, T val, Compare comp, Distance*)
{
    *result = *first;
    __adjust_heap(first, Distance(0), Distance(last - first), val, comp);
}
template <class RandomAccessIter, class T, class Compare>
void __partial_sort(RandomAccessIter first, RandomAccessIter middle, RandomAccessIter last, T*, Compare comp)
{
    make_heap(first, middle, comp);
    for (RandomAccessIter i = middle; i < last; ++i)
        if (comp(*i, *first))
            __pop_heap(first, middle, i, T(*i), comp, (ptrdiff_t*)0);
    sort_heap(first, middle, comp);
}
}   // namespace _STL

static bool Near(float a, float b) { float d = a - b; return d < 0.0001f && d > -0.0001f; }

int main()
{
    float a[10] = { 9.0f, 3.0f, 7.0f, -1.0f, 8.0f, 2.5f, 6.0f, 0.0f, 5.0f, 4.0f };
    const float e[4] = { -1.0f, 0.0f, 2.5f, 3.0f };
    _STL::__partial_sort(a, a + 4, a + 10, (float*)0, _STL::less<float>());
    for (int i = 0; i < 4; ++i)
        if (!Near(a[i], e[i])) { std::printf("00a5a8a0_TEST FAIL\n"); return 1; }
    float sum = 0.0f;                               // the tail keeps the other six values
    for (int i = 4; i < 10; ++i) {
        if (a[i] < 3.0f) { std::printf("00a5a8a0_TEST FAIL\n"); return 1; }
        sum += a[i];
    }
    if (!Near(sum, 39.0f)) { std::printf("00a5a8a0_TEST FAIL\n"); return 1; }
    std::printf("00a5a8a0_TEST PASS\n");
    return 0;
}