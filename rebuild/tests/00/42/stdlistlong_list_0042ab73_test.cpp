// Behaviour test for std::list<long>::~list (0x0042AB73): clear, then free the sentinel when present.
#include <cstdio>
#include <cstdlib>
static int g_cleared = 0; static void* g_freed = 0;
static void MyFree(void* p) { g_freed = p; std::free(p); }
namespace std {
template <class _Tp> class allocator;
template <class _Tp, class _Alloc = allocator<_Tp> > class list {
public:
    void* _M_node;
    void clear() { ++g_cleared; }
    ~list() { clear(); if (_M_node) MyFree(_M_node); }
};
}
int main()
{
    void* n = std::malloc(8);
    { std::list<long> l; l._M_node = n; }
    bool ok = g_cleared == 1 && g_freed == n;
    { std::list<long> l; l._M_node = 0; }
    ok = ok && g_cleared == 2 && g_freed == n;
    std::printf(ok ? "0042ab73_TEST PASS\n" : "0042ab73_TEST FAIL\n");
    return ok ? 0 : 1;
}
