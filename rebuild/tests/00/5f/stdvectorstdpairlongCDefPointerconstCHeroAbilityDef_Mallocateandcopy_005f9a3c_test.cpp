// behaviour test for 0x005F9A3C vector::_M_allocate_and_copy (model: a fresh block holding the copied range)
#include <cstdio>
#include <cstdlib>
static int* AllocateAndCopy(unsigned int n, const int* first, const int* last)
{
    int* result = n ? (int*)std::malloc(n * sizeof(int)) : 0;
    int* cur = result; for (; first != last; ++first, ++cur) *cur = *first;
    return result;
}
int main()
{
    int src[3] = { 1, 2, 3 };
    int* p = AllocateAndCopy(5, src, src + 3);
    int ok = p != 0 && p[0] == 1 && p[2] == 3 && AllocateAndCopy(0, src, src) == 0;
    std::free(p);
    if (!ok) { std::printf("005f9a3c_TEST FAIL\n"); return 1; }
    std::printf("005f9a3c_TEST PASS\n");
    return 0;
}