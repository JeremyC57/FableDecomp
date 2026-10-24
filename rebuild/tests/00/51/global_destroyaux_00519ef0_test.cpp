// Behaviour test for 00519ef0: __destroy_aux over pair<unsigned __int64, std::list<C2DLineF> > -- every element's list
// frees each node and then its header (null header skipped), and the header is relinked to itself before it goes.
#include <cstdio>
#include <cstdlib>

static int g_frees = 0;
static void* g_lastHeaderNext = 0;
static void model_free(void* p) { ++g_frees; std::free(p); }   // retail: free @ 0x00BFEA14

#ifdef _MSC_VER
typedef unsigned __int64 u64;
#else
typedef unsigned long long u64;
#endif

struct C2DLineF { float x1, y1, x2, y2; };
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class T> class list_model {
public:
    _List_node_base* _M_node;
    void clear()
    {
        _List_node_base* cur = _M_node->_M_next;
        while (cur != _M_node) {
            _List_node_base* tmp = cur;
            cur = cur->_M_next;
            model_free(tmp);
        }
        _M_node->_M_next = _M_node;
        _M_node->_M_prev = _M_node;
        g_lastHeaderNext = _M_node->_M_next == _M_node ? (void*)_M_node : 0;
    }
    ~list_model()
    {
        clear();
        if (_M_node)
            model_free(_M_node);
    }
};
struct PairU64LineList {
    u64                    first;
    list_model<C2DLineF>   second;
};
struct __false_type { };
template <class T> inline void _Destroy(T* p) { p->~T(); }

void __destroy_aux(PairU64LineList* first, PairU64LineList* last, const __false_type&)
{
    for (; first != last; ++first)
        _Destroy(&*first);
}

static _List_node_base* make_list(int nodes)
{
    _List_node_base* header = (_List_node_base*)std::malloc(sizeof(_List_node_base));
    header->_M_next = header;
    header->_M_prev = header;
    for (int i = 0; i < nodes; ++i) {
        _List_node_base* n = (_List_node_base*)std::malloc(sizeof(_List_node_base) + sizeof(C2DLineF));
        n->_M_next = header;
        n->_M_prev = header->_M_prev;
        header->_M_prev->_M_next = n;
        header->_M_prev = n;
    }
    return header;
}

int main()
{
    void* raw = std::malloc(sizeof(PairU64LineList) * 3);
    PairU64LineList* v = (PairU64LineList*)raw;
    v[0].first = 1; v[0].second._M_node = make_list(3);
    v[1].first = 2; v[1].second._M_node = make_list(0);
    v[2].first = 3; v[2].second._M_node = make_list(5);
    __false_type tag;
    __destroy_aux(v, v + 3, tag);
    if (g_frees != (3 + 1) + (0 + 1) + (5 + 1) || g_lastHeaderNext == 0) { std::printf("00519ef0_TEST FAIL\n"); return 1; }
    g_frees = 0;
    __destroy_aux(v, v, tag);                               // empty range touches nothing
    if (g_frees != 0) { std::printf("00519ef0_TEST FAIL\n"); return 1; }
    std::free(raw);
    std::printf("00519ef0_TEST PASS\n");
    return 0;
}