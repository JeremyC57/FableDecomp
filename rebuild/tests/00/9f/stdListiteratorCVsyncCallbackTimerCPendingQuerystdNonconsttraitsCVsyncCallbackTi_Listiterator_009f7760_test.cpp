// Behaviour test for 0x009F7760 std::_List_iterator<CPendingQuery, _Nonconst_traits>::_List_iterator(const iterator&)
// (links the source object): the copy goes through the base constructor with the source's node.
#include <cstdio>
#include <new>

struct IDirect3DQuery9;
class CVsyncCallbackTimer {
public:
    struct CPendingQuery { IDirect3DQuery9* Query; long Frame; double TimeStamp; };
};
namespace std {
struct _List_node_base { _List_node_base* _M_next; _List_node_base* _M_prev; };
template <class _Tp> struct _List_node : _List_node_base { _Tp _M_data; };
struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* __x);
};
template <class _Tp> struct _Nonconst_traits { typedef _Tp& reference; };
template <class _Tp, class _Traits> struct _List_iterator : _List_iterator_base {
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    _List_iterator(const iterator& __x);                            // defined in the source object
};
}
static int g_calls = 0;
static std::_List_node_base* g_seen = 0;
std::_List_iterator_base::_List_iterator_base(_List_node_base* __x) : _M_node(__x) { ++g_calls; g_seen = __x; }

typedef std::_List_iterator<CVsyncCallbackTimer::CPendingQuery, std::_Nonconst_traits<CVsyncCallbackTimer::CPendingQuery> > It;

int main()
{
    std::_List_node_base node;
    unsigned char src[4], dst[4];
    *(std::_List_node_base**)src = &node;
    It* copy = new (dst) It(*(It*)src);
    if (g_calls != 1 || g_seen != &node || copy->_M_node != &node || (void*)copy != (void*)dst) { std::printf("009f7760_TEST FAIL\n"); return 1; }
    std::printf("009f7760_TEST PASS\n");
    return 0;
}
