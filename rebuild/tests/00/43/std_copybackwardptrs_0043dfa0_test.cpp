// behaviour test for 0x0043dfa0 std::__copy_backward_ptrs<T*,T*>(first, last, result, __true_type): memmove of
// [first, last) to end at result, returning result - n (result unchanged for an empty range); overlap-safe
#include <cstdio>
#include <cstring>
static long* CopyBackward(const long* first, const long* last, long* result)
{
    const int num = (const char*)last - (const char*)first;
    return num > 0 ? (long*)std::memmove((char*)result - num, first, num) : result;
}
int main()
{
    int fails = 0;
    long a[6] = { 1, 2, 3, 4, 0, 0 };
    long* r = CopyBackward(a, a + 4, a + 6);
    if (r != a + 2) fails |= 1;
    if (a[2] != 1 || a[3] != 2 || a[4] != 3 || a[5] != 4) fails |= 2;
    if (CopyBackward(a, a, a + 3) != a + 3) fails |= 4;
    if (fails) { std::printf("0043dfa0_TEST FAIL %d\n", fails); return 1; }
    std::printf("0043dfa0_TEST PASS\n");
    return 0;
}