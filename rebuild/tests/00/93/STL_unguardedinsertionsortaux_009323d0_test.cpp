// Behaviour test for 0x009323D0 std::__unguarded_insertion_sort_aux<CThing**, CThing*, bool(*)(CThing*,CThing*)>.
#include <cstdio>
#include <cstring>
#define CHECK(c) do { if (!(c)) { std::printf("009323d0_TEST FAIL\n"); return 1; } } while (0)

template <class _RandomAccessIter, class _Tp, class _Compare>
void unguarded_linear_insert_model(_RandomAccessIter __last, _Tp __val, _Compare __comp)
{
    _RandomAccessIter __next = __last;
    --__next;
    while (__comp(__val, *__next)) {
        *__last = *__next;
        __last = __next;
        --__next;
    }
    *__last = __val;
}
// the function under test: STLport __unguarded_insertion_sort_aux
template <class _RandomAccessIter, class _Tp, class _Compare>
void unguarded_insertion_sort_aux_under_test(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Compare __comp)
{
    _RandomAccessIter __i;
    for (__i = __first; __i != __last; ++__i)
        unguarded_linear_insert_model(__i, _Tp(*__i), __comp);
}

class CThing { public: float Height; };
typedef bool (*ThingCompare)(CThing* a, CThing* b);
static bool HeightLess(CThing* a, CThing* b) { return a->Height < b->Height; }

int main()
{
    int i;
    CThing things[6];
    float h[6] = { -10.0f, 5.0f, 1.0f, 3.0f, 2.0f, 4.0f };
    CThing* v[6];
    for (i = 0; i < 6; ++i) { things[i].Height = h[i]; v[i] = &things[i]; }
    unguarded_insertion_sort_aux_under_test(v + 1, v + 6, (CThing**)0, (ThingCompare)HeightLess);
    int order[6] = { 0, 2, 4, 3, 5, 1 };
    for (i = 0; i < 6; ++i) CHECK(v[i] == &things[order[i]]);
    std::printf("009323d0_TEST PASS\n");
    return 0;
}