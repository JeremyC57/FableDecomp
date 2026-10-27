#include <cstdio>
#include <stddef.h>
struct E { int v; };
static int g_calls; static const void* g_tag; static ptrdiff_t* g_dist;
struct random_access_iterator_tag { random_access_iterator_tag() { } };
struct __false_type { __false_type() { } };
E* __copy_backward(E* first, E* last, E* result, const random_access_iterator_tag& t, ptrdiff_t* d)
{
    ++g_calls; g_tag = &t; g_dist = d;
    while (last != first) *--result = *--last;
    return result;
}
inline E* __copy_backward_ptrs(E* first, E* last, E* result, const __false_type&)
{
    return __copy_backward(first, last, result, random_access_iterator_tag(), (ptrdiff_t*)0);
}
int main()
{
    E a[6] = { {1}, {2}, {3}, {0}, {0}, {0} };
    E* r = __copy_backward_ptrs(a, a + 3, a + 5, __false_type());
    if (r != a + 2 || a[2].v != 1 || a[3].v != 2 || a[4].v != 3 || g_calls != 1 || g_dist != 0 || g_tag == 0) { std::printf("00518f60_TEST FAIL\n"); return 1; }
    std::printf("00518f60_TEST PASS\n");
    return 0;
}