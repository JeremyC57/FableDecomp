// Behaviour test for the m_mapScreens tree destructor (0x0059B098): clear(), then free the header when present.
#include <cstdio>
#include <cstdlib>
static int g_cleared = 0; static void* g_freed = 0;
static void MyFree(void* p) { g_freed = p; std::free(p); }
enum FRONTEND_SCREEN { FRONTEND_SCREEN_NONE };
namespace NUISystem { class CComponent; }
namespace std {
template <class _Tp> class allocator;
template <class _T1, class _T2> struct pair;
template <class _Pair> struct _Select1st;
template <class _Tp> struct less;
struct _Rb_tree_node_base;
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree {
public:
    _Rb_tree_node_base* _M_header;
    unsigned int        _M_node_count;
    void clear() { ++g_cleared; }
    ~_Rb_tree() { clear(); if (_M_header) MyFree(_M_header); }
};
}
typedef std::pair<const FRONTEND_SCREEN, NUISystem::CComponent*> TScreenValue;
typedef std::_Rb_tree<FRONTEND_SCREEN, TScreenValue, std::_Select1st<TScreenValue>, std::less<FRONTEND_SCREEN>,
                      std::allocator<TScreenValue> > TTree;
int main()
{
    void* h = std::malloc(16);
    { TTree t; t._M_header = (std::_Rb_tree_node_base*)h; t._M_node_count = 2; }
    bool ok = g_cleared == 1 && g_freed == h;
    { TTree t; t._M_header = 0; t._M_node_count = 0; }
    ok = ok && g_cleared == 2 && g_freed == h;
    std::printf(ok ? "0059b098_TEST PASS\n" : "0059b098_TEST FAIL\n");
    return ok ? 0 : 1;
}
