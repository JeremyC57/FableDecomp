#include <cstdio>
#include <cstdlib>
// Behaviour model: n elements are allocated (none for n == 0) and [first, last) is copied to the front of the block.
static int* alloc_copy(unsigned n, const int* f, const int* l)
{
    int* r = n ? (int*)std::malloc(n * sizeof(int)) : 0;
    int* c = r;
    for (; f != l; ++f, ++c) *c = *f;
    return r;
}
int main()
{
    int src[3] = { 4, 5, 6 };
    int* p = alloc_copy(5, src, src + 3);
    if (!p || p[0] != 4 || p[2] != 6) { std::printf("0082c450_TEST FAIL\n"); return 1; }
    std::free(p);
    if (alloc_copy(0, src, src) != 0) { std::printf("0082c450_TEST FAIL\n"); return 1; }
    std::printf("0082c450_TEST PASS\n");
    return 0;
}