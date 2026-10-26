// Test for 0x0048CAF0 std::_Deque_base<C2DVector>::~_Deque_base: with a map, frees nodes [start node, finish node] then the map;
// without a map, frees nothing.
#include <stdio.h>
static void* g_freed[16]; static int g_nfreed;
static void Free(void* p) { g_freed[g_nfreed++] = p; }
class C2DVector { public: float X, Y; };
struct Iter { C2DVector* cur; C2DVector* first; C2DVector* last; C2DVector** node; };
struct Base {
    Iter _M_start, _M_finish; C2DVector** _M_map; unsigned int _M_map_size;
    void _M_destroy_nodes(C2DVector** s, C2DVector** f) { for (C2DVector** n = s; n < f; ++n) if (*n) Free(*n); }
    ~Base() { if (_M_map) { _M_destroy_nodes(_M_start.node, _M_finish.node + 1); if (_M_map) Free(_M_map); } }
};
int main()
{
    if (sizeof(Base) != 0x28 || (char*)&((Base*)0)->_M_map - (char*)0 != 0x20) { printf("0048caf0_TEST FAIL layout\n"); return 1; }
    C2DVector a[16], b[16];
    C2DVector* map[8] = { 0, 0, 0, a, b, 0, 0, 0 };
    {
        Base d; d._M_start.node = map + 3; d._M_finish.node = map + 4; d._M_map = map; d._M_map_size = 8;
    }
    if (g_nfreed != 3 || g_freed[0] != a || g_freed[1] != b || g_freed[2] != (void*)map) { printf("0048caf0_TEST FAIL free\n"); return 1; }
    g_nfreed = 0;
    { Base e; e._M_map = 0; e._M_map_size = 0; e._M_start.node = 0; e._M_finish.node = 0; }
    if (g_nfreed != 0) { printf("0048caf0_TEST FAIL nomap\n"); return 1; }
    printf("0048caf0_TEST PASS\n");
    return 0;
}
