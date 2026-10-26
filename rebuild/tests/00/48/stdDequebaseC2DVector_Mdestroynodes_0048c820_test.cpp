// Test for 0x0048C820 std::_Deque_base<C2DVector>::_M_destroy_nodes: frees every non-null node in [nstart, nfinish), nothing else.
#include <stdio.h>
static void* g_freed[16]; static int g_nfreed;
static void Free(void* p) { g_freed[g_nfreed++] = p; }
class C2DVector { public: float X, Y; };
template <class _Tp> class allocator { public: void deallocate(_Tp* p, unsigned int) { if (p != 0) Free(p); } };
template <class _Tp, class _Alloc> class _Deque_base {
public:
    _Alloc _M_map_size;
    static unsigned int buffer_size() { return 16; }
    void _M_destroy_nodes(_Tp** nstart, _Tp** nfinish) { for (_Tp** n = nstart; n < nfinish; ++n) _M_map_size.deallocate(*n, buffer_size()); }
};
int main()
{
    C2DVector a[16], b[16], c[16];
    C2DVector* map[5] = { a, b, 0, c, a };
    _Deque_base<C2DVector, allocator<C2DVector> > d;
    d._M_destroy_nodes(map + 0, map + 4);
    if (g_nfreed != 3 || g_freed[0] != a || g_freed[1] != b || g_freed[2] != c) { printf("0048c820_TEST FAIL range\n"); return 1; }
    g_nfreed = 0; d._M_destroy_nodes(map + 2, map + 2);
    if (g_nfreed != 0) { printf("0048c820_TEST FAIL empty\n"); return 1; }
    d._M_destroy_nodes(map + 3, map + 2);
    if (g_nfreed != 0) { printf("0048c820_TEST FAIL reversed\n"); return 1; }
    printf("0048c820_TEST PASS\n");
    return 0;
}
