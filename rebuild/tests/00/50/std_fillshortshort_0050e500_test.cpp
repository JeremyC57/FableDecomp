// Behaviour test for 0x0050E500 std::fill<short*,short>: every slot of [first, last) receives the value, the
// slots outside the range are untouched, and an empty range writes nothing.
#include <cstdio>

template <class ForwardIter, class T>
void fill_model(ForwardIter first, ForwardIter last, const T& val)
{
    for (; first != last; ++first)
        *first = val;
}

int main()
{
    short a[8] = { 1, 2, 3, 4, 5, 6, 7, 8 };
    short v = -5;
    fill_model(a + 2, a + 6, v);
    static const short want[8] = { 1, 2, -5, -5, -5, -5, 7, 8 };
    for (int i = 0; i < 8; ++i)
        if (a[i] != want[i]) { std::printf("0050e500_TEST FAIL\n"); return 1; }
    fill_model(a + 3, a + 3, (short)99);
    if (a[3] != -5) { std::printf("0050e500_TEST FAIL\n"); return 1; }
    // the value is read through its reference each pass: filling with a slot inside the range is stable
    fill_model(a, a + 8, a[0]);
    for (int i = 0; i < 8; ++i)
        if (a[i] != 1) { std::printf("0050e500_TEST FAIL\n"); return 1; }
    std::printf("0050e500_TEST PASS\n");
    return 0;
}