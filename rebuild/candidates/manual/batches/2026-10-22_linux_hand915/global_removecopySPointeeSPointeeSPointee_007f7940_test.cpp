#include <cstdio>
// Behaviour test for 007f7940: remove_copy copies the pointers that differ from the value and returns the new end.
template <class In, class Out, class T>
Out remove_copy(In first, In last, Out result, const T& val)
{
    for ( ; first != last; ++first)
        if (!(*first == val)) { *result = *first; ++result; }
    return result;
}
int main()
{
    int a = 1, b = 2, c = 3;
    int* src[5] = { &a, &b, &a, &c, &a };
    int* dst[5] = { 0, 0, 0, 0, 0 };
    int** end = remove_copy(src, src + 5, dst, &a);
    int ok = (end == dst + 2 && dst[0] == &b && dst[1] == &c && dst[2] == 0);
    ok = ok && remove_copy(src, src, dst, &b) == dst;
    if (!ok) { std::printf("007f7940_TEST FAIL\n"); return 1; }
    std::printf("007f7940_TEST PASS\n");
    return 0;
}
