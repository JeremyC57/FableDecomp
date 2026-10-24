// Behaviour test for 00c13d70: std::copy_backward over scalar pointers -- one memmove of [first, last) so it ends at dest; returns dest - (last - first); overlapping ranges move correctly
#include <cstdio>
#include <cstring>

template<class In, class Out>
Out CopyBackward(In first, In last, Out dest)
{
    int off = last - first;
    return (Out)std::memmove(&*dest - off, &*first, off * sizeof(*first));
}

int main()
{
    long a[8] = { 1, 2, 3, 4, 5, 6, 0, 0 };
    long* r = CopyBackward(a + 0, a + 6, a + 8);          // shift right by two, overlapping
    if (r != a + 2) { std::printf("00c13d70_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 6; ++i)
        if (a[i + 2] != (long)(i + 1)) { std::printf("00c13d70_TEST FAIL\n"); return 1; }
    long b[3] = { 7, 8, 9 };
    long c[3] = { 0, 0, 0 };
    if (CopyBackward(b + 0, b + 3, c + 3) != c || c[0] != 7 || c[2] != 9) { std::printf("00c13d70_TEST FAIL\n"); return 1; }
    if (CopyBackward(b + 1, b + 1, c + 2) != c + 2) { std::printf("00c13d70_TEST FAIL\n"); return 1; }   // empty range
    std::printf("00c13d70_TEST PASS\n");
    return 0;
}