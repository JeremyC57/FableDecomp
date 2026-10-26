// Behaviour test for 00a37af0 (std::__destroy_aux over SListRecord): each record destroys its name, then frees every list node and the list header.
#include <cstdio>
#include <cstdlib>
#include <new>
static int g_frees = 0;
static int g_strings = 0;
static void test_free(void* p) { ++g_frees; std::free(p); }
class CCharString { public:
    void* m_pData;
    CCharString() : m_pData(0) { }
    ~CCharString() { ++g_strings; }                  // stands in for 0x0099EAE0
};
namespace std2 {
struct __false_type { };
template <class T> inline void _Destroy(T* p) { p->~T(); }
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class T> struct _List_node : _List_node_base {
    T _M_data;
};
template <class T> struct allocator {
    void deallocate(T* p, unsigned int) { if (p) test_free(p); }
};
template <class T, class A> struct _STLP_alloc_proxy : A {
    T _M_data;
};
template <class T> struct _List_base {
    _STLP_alloc_proxy<_List_node<T>*, allocator<_List_node<T> > > _M_node;
    _List_base()
    {
        _M_node._M_data = static_cast<_List_node<T>*>(std::malloc(sizeof(_List_node<T>)));
        _M_node._M_data->_M_next = _M_node._M_data;
        _M_node._M_data->_M_prev = _M_node._M_data;
    }
    void push_back(const T& v)
    {
        _List_node<T>* n = static_cast<_List_node<T>*>(std::malloc(sizeof(_List_node<T>)));
        n->_M_data = v;
        n->_M_next = _M_node._M_data;
        n->_M_prev = _M_node._M_data->_M_prev;
        _M_node._M_data->_M_prev->_M_next = n;
        _M_node._M_data->_M_prev = n;
    }
    void clear()
    {
        _List_node<T>* cur = (_List_node<T>*)_M_node._M_data->_M_next;
        while (cur != _M_node._M_data) {
            _List_node<T>* tmp = cur;
            cur = (_List_node<T>*)cur->_M_next;
            _M_node.deallocate(tmp, 1);
        }
        _M_node._M_data->_M_next = _M_node._M_data;
        _M_node._M_data->_M_prev = _M_node._M_data;
    }
    ~_List_base()
    {
        clear();
        _M_node.deallocate(_M_node._M_data, 1);
    }
};
template <class T> struct list : _List_base<T> { };
}
struct SListRecord {
    std2::list<long> m_items;
    long m_values[3];
    CCharString m_name;
};
void __destroy_aux(SListRecord* first, SListRecord* last, const std2::__false_type&)
{
    for (; first != last; ++first)
        std2::_Destroy(&*first);
}
union UStorage { double d; void* p; char bytes[sizeof(SListRecord) * 2]; };
int main()
{
    UStorage storage;
    SListRecord* records = reinterpret_cast<SListRecord*>(storage.bytes);
    new (&records[0]) SListRecord();
    new (&records[1]) SListRecord();
    records[0].m_items.push_back(1);
    records[0].m_items.push_back(2);
    records[1].m_items.push_back(3);
    std2::__false_type tag;
    __destroy_aux(records, records, tag);
    if (g_frees != 0 || g_strings != 0) { std::printf("00a37af0_TEST FAIL\n"); return 1; }
    __destroy_aux(records, records + 2, tag);
    // 3 nodes + 2 headers freed, 2 names destroyed
    if (g_frees != 5 || g_strings != 2) { std::printf("00a37af0_TEST FAIL\n"); return 1; }
    std::printf("00a37af0_TEST PASS\n");
    return 0;
}