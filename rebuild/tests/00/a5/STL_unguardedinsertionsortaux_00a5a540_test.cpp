// Behaviour test for 0x00A5A540 std::__unguarded_insertion_sort_aux<float*, float, less<float> >
#include <cstdio>

namespace std {
template <class T> struct less {
    bool operator()(const T& x, const T& y) const { return x < y; }
};
template <class RandomAccessIter, class T, class Compare>
inline void __unguarded_linear_insert(RandomAccessIter last, T val, Compare comp)
{
    RandomAccessIter next = last;
    --next;
    while (comp(val, *next)) {
        *last = *next;
        last = next;
        --next;
    }
    *last = val;
}
template <class RandomAccessIter, class T, class Compare>
void __unguarded_insertion_sort_aux(RandomAccessIter first, RandomAccessIter last, T*, Compare comp)
{
    for (RandomAccessIter i = first; i != last; ++i)
        __unguarded_linear_insert(i, T(*i), comp);
}
}   // namespace std

static bool Near(float a, float b) { float d = a - b; return d < 0.0001f && d > -0.0001f; }

int main()
{
    // a[0] is the sentinel that makes the unguarded pass safe (no element sorts below it)
    float a[8] = { -100.0f, 5.0f, 3.0f, 9.0f, 1.0f, 3.0f, -2.0f, 7.5f };
    const float expect[8] = { -100.0f, -2.0f, 1.0f, 3.0f, 3.0f, 5.0f, 7.5f, 9.0f };
    std::__unguarded_insertion_sort_aux(a + 1, a + 8, (float*)0, std::less<float>());
    for (int i = 0; i < 8; ++i)
        if (!Near(a[i], expect[i])) { std::printf("00a5a540_TEST FAIL\n"); return 1; }
    // an already-sorted range is left untouched
    float b[4] = { 0.0f, 1.0f, 2.0f, 3.0f };
    std::__unguarded_insertion_sort_aux(b + 1, b + 4, (float*)0, std::less<float>());
    for (int i = 0; i < 4; ++i)
        if (!Near(b[i], (float)i)) { std::printf("00a5a540_TEST FAIL\n"); return 1; }
    std::printf("00a5a540_TEST PASS\n");
    return 0;
}