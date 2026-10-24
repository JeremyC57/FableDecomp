// Behaviour test for 0x00A61A80 CDirectInputDeviceLists_00a61b10::ReleaseAll()
#include <cstdio>
#include <cstdlib>

static int g_order[8];
static int g_released = 0;

struct IUnknown {                                   // COM stand-in: Release is the third virtual
    int Id;
    virtual long QueryInterface(const void*, void**) { return -1; }
    virtual unsigned long AddRef() { return 1; }
    virtual unsigned long Release() { g_order[g_released++] = Id; return 0; }
    virtual ~IUnknown() { }
};
struct SDirectInputInterfaceEntry {
    IUnknown*     Interface;
    unsigned long Data;
};
namespace _STL {
template <class _Tp> class allocator {
public:
    allocator() {}
    allocator(const allocator<_Tp>&) {}
    _Tp* allocate(unsigned int n) { return n != 0 ? (_Tp*)std::malloc(n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* p, unsigned int) { std::free(p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) {}
};
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class _Tp> struct _List_node : _List_node_base {
    _Tp _M_data;
};
template <class _Tp, class _Alloc>
class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    typedef _Alloc allocator_type;
    typedef allocator<_Node> _Node_allocator_type;
    _List_base(const allocator_type&) : _M_node(_Node_allocator_type(), (_Node*)0)
    {
        _Node* __n = _M_node.allocate(1);
        __n->_M_next = __n;
        __n->_M_prev = __n;
        _M_node._M_data = __n;
    }
    ~_List_base()                                   // test-only teardown: free every node and the sentinel
    {
        _List_node_base* n = _M_node._M_data->_M_next;
        while (n != _M_node._M_data) { _List_node_base* next = n->_M_next; std::free(n); n = next; }
        std::free(_M_node._M_data);
    }
    _STLP_alloc_proxy<_Node*, _Node, _Node_allocator_type> _M_node;
};

template <class _Tp> struct _Nonconst_traits {
    typedef _Tp& reference;
    typedef _Nonconst_traits<_Tp> _Non_const_traits;
};
struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* __x) : _M_node(__x) {}
    void _M_incr() { _M_node = _M_node->_M_next; }
    void _M_decr() { _M_node = _M_node->_M_prev; }
};
template <class _Tp, class _Traits> struct _List_iterator : _List_iterator_base {
    typedef _List_iterator<_Tp, typename _Traits::_Non_const_traits> iterator;
    typedef _List_iterator<_Tp, _Traits> _Self;
    typedef typename _Traits::reference reference;
    _List_iterator(_List_node_base* __x) : _List_iterator_base(__x) {}
    _List_iterator(const iterator& __x) : _List_iterator_base(__x._M_node) {}
    reference operator*() const { return ((_List_node<_Tp>*)_M_node)->_M_data; }
    _Self& operator--() { this->_M_decr(); return *this; }
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class list : public _List_base<_Tp, _Alloc> {
public:
    typedef _Alloc allocator_type;
    typedef _List_node<_Tp> _Node;
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    typedef unsigned int size_type;
    explicit list(const allocator_type& __a = allocator_type()) : _List_base<_Tp, _Alloc>(__a) {}
    iterator end() { return iterator(this->_M_node._M_data); }
    size_type size() const
    {
        size_type __result = 0;
        for (_List_node_base* __n = this->_M_node._M_data->_M_next; __n != this->_M_node._M_data; __n = __n->_M_next)
            ++__result;
        return __result;
    }
    _Tp& back() { iterator __tmp = end(); --__tmp; return *__tmp; }
    iterator erase(iterator __position)
    {
        _List_node_base* __next_node = __position._M_node->_M_next;
        _List_node_base* __prev_node = __position._M_node->_M_prev;
        _Node* __n = (_Node*)__position._M_node;
        __prev_node->_M_next = __next_node;
        __next_node->_M_prev = __prev_node;
        this->_M_node.deallocate(__n, 1);
        return iterator(__next_node);
    }
    void pop_back() { iterator __tmp = end(); erase(--__tmp); }
    void push_back_for_test(const _Tp& x)           // test helper
    {
        _Node* n = (_Node*)std::malloc(sizeof(_Node));
        n->_M_data = x;
        _List_node_base* s = this->_M_node._M_data;
        n->_M_next = s;
        n->_M_prev = s->_M_prev;
        s->_M_prev->_M_next = n;
        s->_M_prev = n;
    }
};
}   // namespace _STL

class CDirectInputDeviceLists_00a61b10 {
public:
    _STL::list<SDirectInputInterfaceEntry> m_devices;
    _STL::list<SDirectInputInterfaceEntry> m_effects;
    long          m_enabled;
    long          m_count;
    CDirectInputDeviceLists_00a61b10() : m_enabled(1), m_count(0) { }
    void ReleaseAll()
    {
        while (m_effects.size() > 0) {
            m_effects.back().Interface->Release();
            m_effects.pop_back();
        }
        while (m_devices.size() > 0) {
            m_devices.back().Interface->Release();
            m_devices.pop_back();
        }
    }
};

static bool Empty(_STL::list<SDirectInputInterfaceEntry>& l)
{
    _STL::_List_node_base* s = l._M_node._M_data;
    return s != 0 && s->_M_next == s && s->_M_prev == s && l.size() == 0;
}

int main()
{
    IUnknown obj[5];
    for (int i = 0; i < 5; ++i) obj[i].Id = i + 1;
    CDirectInputDeviceLists_00a61b10 lists;
    SDirectInputInterfaceEntry e;
    e.Data = 0;
    for (int i = 0; i < 3; ++i) { e.Interface = &obj[i]; lists.m_devices.push_back_for_test(e); }   // devices 1, 2, 3
    for (int i = 3; i < 5; ++i) { e.Interface = &obj[i]; lists.m_effects.push_back_for_test(e); }   // effects 4, 5
    lists.ReleaseAll();
    const int expect[5] = { 5, 4, 3, 2, 1 };            // the second list first, each from the back
    bool ok = g_released == 5 && Empty(lists.m_devices) && Empty(lists.m_effects);
    for (int i = 0; ok && i < 5; ++i) ok = g_order[i] == expect[i];
    lists.ReleaseAll();                                  // empty lists: nothing more is released
    ok = ok && g_released == 5;
    if (!ok) { std::printf("00a61a80_TEST FAIL\n"); return 1; }
    std::printf("00a61a80_TEST PASS\n");
    return 0;
}