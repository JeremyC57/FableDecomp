// Behaviour test for 0x00A5A860 _STL::__insertion_sort<float*, less<float> >
#include <cstdio>
#include <cstring>
#include <cstddef>

namespace _STL {
template <class T> struct less {
    bool operator()(const T& x, const T& y) const { return x < y; }
};
template <class RandomAccessIter, class T, class Compare>
void __linear_insert(RandomAccessIter first, RandomAccessIter last, T val, Compare comp)   // 0x00A5A770 stand-in
{
    if (comp(val, *first)) {
        std::memmove(first + 1, first, (char*)last - (char*)first);
        *first = val;
    }
    else {
        RandomAccessIter next = last;
        --next;
        while (comp(val, *next)) {
            *last = *next;
            last = next;
            --next;
        }
        *last = val;
    }
}
template <class RandomAccessIter, class Compare>
void __insertion_sort(RandomAccessIter first, RandomAccessIter last, Compare comp)
{
    if (first == last) return;
    for (RandomAccessIter i = first + 1; i != last; ++i)
        __linear_insert(first, i, *i, comp);
}
}   // namespace _STL

static bool Near(float a, float b) { float d = a - b; return d < 0.0001f && d > -0.0001f; }

int main()
{
    float a[9] = { 5.0f, -1.0f, 3.5f, 9.0f, 0.0f, 3.5f, -7.25f, 2.0f, 8.0f };
    const float e[9] = { -7.25f, -1.0f, 0.0f, 2.0f, 3.5f, 3.5f, 5.0f, 8.0f, 9.0f };
    _STL::__insertion_sort(a, a + 9, _STL::less<float>());
    for (int i = 0; i < 9; ++i)
        if (!Near(a[i], e[i])) { std::printf("00a5a860_TEST FAIL\n"); return 1; }
    float one = 4.0f;                               // empty and single-element ranges are no-ops
    _STL::__insertion_sort(&one, &one, _STL::less<float>());
    _STL::__insertion_sort(&one, &one + 1, _STL::less<float>());
    if (!Near(one, 4.0f)) { std::printf("00a5a860_TEST FAIL\n"); return 1; }
    std::printf("00a5a860_TEST PASS\n");
    return 0;
}