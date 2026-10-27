#include <cstdio>
#include <stddef.h>
struct E { int v; };
static int g_calls; static const void* g_tag; static ptrdiff_t* g_dist;
struct random_access_iterator_tag { random_access_iterator_tag() { } };
struct __false_type { __false_type() { } };
E* __copy(const E* first, const E* last, E* result, const random_access_iterator_tag& t, ptrdiff_t* d)
{
    ++g_calls; g_tag = &t; g_dist = d;
    for (; first != last; ++first, ++result) *result = *first;
    return result;
}
inline E* __copy_ptrs(const E* first, const E* last, E* result, const __false_type&)
{
    return __copy(first, last, result, random_access_iterator_tag(), (ptrdiff_t*)0);
}
inline E* copy_(const E* first, const E* last, E* result) { return __copy_ptrs(first, last, result, __false_type()); }
int main()
{
    E a[6] = { {0}, {0}, {0}, {7}, {8}, {9} };
    E* r = copy_(a + 3, a + 6, a);
    if (r != a + 3 || a[0].v != 7 || a[1].v != 8 || a[2].v != 9 || g_calls != 1 || g_dist != 0 || g_tag == 0) { std::printf("005190b0_TEST FAIL\n"); return 1; }
    std::printf("005190b0_TEST PASS\n");
    return 0;
}