// behaviour test for 0x004e2589 std::vector<CDefIndex >::_M_allocate_and_copy: n elements are allocated (none for n == 0)
// and [first, last) is copied into the front of the block, which is returned
#include <cstdio>
#include <cstdlib>
static int gAllocs = 0;
static int* AllocateAndCopy(unsigned int n, const int* first, const int* last)
{
    int* result = n != 0 ? (++gAllocs, (int*)std::malloc(n * sizeof(int))) : 0;
    int* cur = result;
    for (; first != last; ++first, ++cur) if (cur) *cur = *first;
    return result;
}
int main()
{
    int fails = 0;
    int src[3] = { 4, 5, 6 };
    int* p = AllocateAndCopy(5, src, src + 3);
    if (!p || gAllocs != 1 || p[0] != 4 || p[2] != 6) fails |= 1;
    std::free(p);
    int* q = AllocateAndCopy(0, src, src);
    if (q || gAllocs != 1) fails |= 2;
    if (fails) { std::printf("004e2589_TEST FAIL %d\n", fails); return 1; }
    std::printf("004e2589_TEST PASS\n");
    return 0;
}