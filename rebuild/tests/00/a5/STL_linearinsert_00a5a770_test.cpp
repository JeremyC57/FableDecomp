// Behaviour test for 0x00A5A770 std::__linear_insert<float*, float, less<float> >
#include <cstdio>
#include <cstring>
#include <cstddef>

namespace std {
template <class T> struct less {
    bool operator()(const T& x, const T& y) const { return x < y; }
};
inline void* __copy_trivial_backward(const void* first, const void* last, void* result)
{
    const ptrdiff_t num = (const char*)last - (const char*)first;
    return (num > 0) ? std::memmove((char*)result - num, first, num) : result;
}
template <class T>
inline T* copy_backward(T* first, T* last, T* result)
{
    return (T*)__copy_trivial_backward(first, last, result);
}
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
inline void __linear_insert(RandomAccessIter first, RandomAccessIter last, T val, Compare comp)
{
    if (comp(val, *first)) {
        copy_backward(first, last, last + 1);
        *first = val;
    }
    else
        __unguarded_linear_insert(last, val, comp);
}
}   // namespace std

static bool Near(float a, float b) { float d = a - b; return d < 0.0001f && d > -0.0001f; }

static bool Same(const float* a, const float* b, int n)
{
    for (int i = 0; i < n; ++i)
        if (!Near(a[i], b[i])) return false;
    return true;
}

int main()
{
    // val below the first element: the whole sorted prefix shifts up one slot
    float a[5] = { 2.0f, 4.0f, 6.0f, 8.0f, 1.0f };
    const float ea[5] = { 1.0f, 2.0f, 4.0f, 6.0f, 8.0f };
    std::__linear_insert(a, a + 4, a[4], std::less<float>());
    // val inside the range: unguarded insert from the back
    float b[5] = { 2.0f, 4.0f, 6.0f, 8.0f, 5.0f };
    const float eb[5] = { 2.0f, 4.0f, 5.0f, 6.0f, 8.0f };
    std::__linear_insert(b, b + 4, b[4], std::less<float>());
    // val above every element stays put
    float c[3] = { 1.0f, 2.0f, 9.0f };
    const float ec[3] = { 1.0f, 2.0f, 9.0f };
    std::__linear_insert(c, c + 2, c[2], std::less<float>());
    if (!Same(a, ea, 5) || !Same(b, eb, 5) || !Same(c, ec, 3)) { std::printf("00a5a770_TEST FAIL\n"); return 1; }
    std::printf("00a5a770_TEST PASS\n");
    return 0;
}