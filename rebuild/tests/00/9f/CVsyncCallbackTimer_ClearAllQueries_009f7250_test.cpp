// Behaviour test for 0x009F7250 CVsyncCallbackTimer::ClearAllQueries: Release() and pop the back query of PendingQueories until it is empty, then the same for FreeQueoryObjects (back to front).
#include <cstdio>
#include <cstdlib>
#include <new>

static int gNodesAllocated = 0, gNodesFreed = 0;

namespace std {

struct input_iterator_tag { input_iterator_tag() {} };
struct bidirectional_iterator_tag : input_iterator_tag { bidirectional_iterator_tag() {} };

struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};

template <class _Tp> struct _List_node : _List_node_base {
    _Tp _M_data;
};

struct _List_iterator_base {                        // out of line in retail (0x0041C780 / 0x0041C7A0 / 0x0041C7B0 / 0x0041C7C0)
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* __x) : _M_node(__x) {}
    void _M_incr() { _M_node = _M_node->_M_next; }
    void _M_decr() { _M_node = _M_node->_M_prev; }
    bool operator!=(const _List_iterator_base& __y) const { return _M_node != __y._M_node; }
};

template <class _Tp> struct _Nonconst_traits { typedef _Tp& reference; };
template <class _Tp> struct _Const_traits { typedef const _Tp& reference; };

template <class _Tp, class _Traits> struct _List_iterator : _List_iterator_base {
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    typedef typename _Traits::reference reference;
    typedef _List_node<_Tp> _Node;
    _List_iterator(_Node* __x) : _List_iterator_base(__x) {}
    _List_iterator(const iterator& __x) : _List_iterator_base(__x._M_node) {}
    reference operator*() const { return ((_Node*)_M_node)->_M_data; }
    _List_iterator& operator++() { _M_incr(); return *this; }
    _List_iterator& operator--() { _M_decr(); return *this; }
};

template <class _InputIterator>
int __distance(const _InputIterator& __first, const _InputIterator& __last, const input_iterator_tag&)
{
    int __n = 0;
    _InputIterator __it(__first);
    while (__it != __last) {
        ++__it;
        ++__n;
    }
    return __n;
}

template <class _InputIterator>
int distance(const _InputIterator& __first, const _InputIterator& __last)
{
    return __distance(__first, __last, bidirectional_iterator_tag());
}

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _Tp> class allocator {
public:
    allocator() {}
    allocator(const allocator&) {}
    template <class _Tp1> allocator(const allocator<_Tp1>&) {}
    _Tp* allocate(unsigned int __n, const void* = 0)
    {
        if (__n == 0)
            return 0;
        ++gNodesAllocated;
        return (_Tp*)std::malloc(__n * sizeof(_Tp));
    }
    void deallocate(_Tp* __p, unsigned int) { if (__p) { ++gNodesFreed; std::free(__p); } }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc> class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) {}
};

template <class _Tp, class _Alloc> class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_node;
    _List_base(const _Alloc& __a) : _M_node(allocator<_Node>(__a), (_Node*)0)
    {
        _Node* __n = _M_node.allocate(1);
        __n->_M_next = __n;
        __n->_M_prev = __n;
        _M_node._M_data = __n;
    }
    void clear()
    {
        _Node* __cur = (_Node*)_M_node._M_data->_M_next;
        while (__cur != _M_node._M_data) {
            _Node* __tmp = __cur;
            __cur = (_Node*)__cur->_M_next;
            _M_node.deallocate(__tmp, 1);
        }
        _M_node._M_data->_M_next = _M_node._M_data;
        _M_node._M_data->_M_prev = _M_node._M_data;
    }
};

template <class _Tp, class _Alloc> class list : public _List_base<_Tp, _Alloc> {
public:
    typedef _List_node<_Tp> _Node;
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    typedef _List_iterator<_Tp, _Const_traits<_Tp> > const_iterator;
    list() : _List_base<_Tp, _Alloc>(_Alloc()) {}
    _Node* _M_create_node(const _Tp& __x)
    {
        _Node* __tmp = this->_M_node.allocate(1);
        _Construct(&__tmp->_M_data, __x);
        return __tmp;
    }
    iterator begin() { return iterator((_Node*)(this->_M_node._M_data->_M_next)); }
    const_iterator begin() const { return const_iterator((_Node*)(this->_M_node._M_data->_M_next)); }
    iterator end() { return iterator(this->_M_node._M_data); }
    const_iterator end() const { return const_iterator(this->_M_node._M_data); }
    unsigned int size() const
    {
        unsigned int __result = distance(begin(), end());
        return __result;
    }
    _Tp& back() { iterator __tmp = end(); --__tmp; return *__tmp; }
    iterator insert(iterator __position, const _Tp& __x)
    {
        _Node* __tmp = _M_create_node(__x);
        _List_node_base* __n = __position._M_node;
        _List_node_base* __prev = __n->_M_prev;
        __tmp->_M_next = __n;
        __tmp->_M_prev = __prev;
        __prev->_M_next = __tmp;
        __n->_M_prev = __tmp;
        return __tmp;
    }
    void push_front(const _Tp& __x) { insert(begin(), __x); }
    void push_back(const _Tp& __x) { insert(end(), __x); }
    iterator erase(iterator __position)
    {
        _List_node_base* __next_node = __position._M_node->_M_next;
        _List_node_base* __prev_node = __position._M_node->_M_prev;
        _Node* __n = (_Node*)__position._M_node;
        __prev_node->_M_next = __next_node;
        __next_node->_M_prev = __prev_node;
        this->_M_node.deallocate(__n, 1);
        return iterator((_Node*)__next_node);
    }
    void pop_back()
    {
        iterator __tmp = end();
        erase(--__tmp);
    }
};

}

static int gReleaseOrder[16];
static int gReleaseCount = 0;

struct IDirect3DQuery9 {
    int m_id;
    unsigned long m_refs;
    virtual unsigned long Release() { gReleaseOrder[gReleaseCount++] = m_id; return --m_refs; }
    virtual ~IDirect3DQuery9() {}
};

class CVsyncCallbackTimer {
public:
    struct CPendingQuery {
        IDirect3DQuery9* Query;
        long             Frame;
        double           TimeStamp;
    };
    typedef std::list<CPendingQuery, std::allocator<CPendingQuery> > CQueryList;
    void* Device;
    CQueryList FreeQueoryObjects;
    CQueryList PendingQueories;
    long PendingFrame;
    long FinishedFrame;
    void ClearAllQueries()
    {
        while (PendingQueories.size() > 0) {
            PendingQueories.back().Query->Release();
            PendingQueories.pop_back();
        }
        while (FreeQueoryObjects.size() > 0) {
            FreeQueoryObjects.back().Query->Release();
            FreeQueoryObjects.pop_back();
        }
    }
    void OnPreDeviceReset() { ClearAllQueries(); }
};

typedef CVsyncCallbackTimer::CPendingQuery CPendingQuery;
typedef CVsyncCallbackTimer::CQueryList CQueryList;

static CPendingQuery MakeQuery(IDirect3DQuery9* q, long frame)
{
    CPendingQuery p;
    p.Query = q;
    p.Frame = frame;
    p.TimeStamp = frame * 0.5;
    return p;
}

int main()
{
    IDirect3DQuery9 q[5];
    for (int i = 0; i < 5; ++i) { q[i].m_id = i; q[i].m_refs = 1; }
    bool ok = true;
    {
        CVsyncCallbackTimer t;
        t.PendingQueories.push_back(MakeQuery(&q[0], 10));
        t.PendingQueories.push_back(MakeQuery(&q[1], 11));
        t.PendingQueories.push_back(MakeQuery(&q[2], 12));
        t.FreeQueoryObjects.push_back(MakeQuery(&q[3], 0));
        t.FreeQueoryObjects.push_back(MakeQuery(&q[4], 0));
        t.ClearAllQueries();
        ok = t.PendingQueories.size() == 0 && t.FreeQueoryObjects.size() == 0 && gReleaseCount == 5
          && gReleaseOrder[0] == 2 && gReleaseOrder[1] == 1 && gReleaseOrder[2] == 0
          && gReleaseOrder[3] == 4 && gReleaseOrder[4] == 3 && q[0].m_refs == 0 && q[4].m_refs == 0
          && gNodesFreed == 5;
        t.ClearAllQueries();
        ok = ok && gReleaseCount == 5;
    }
    if (!ok) { std::printf("009f7250_TEST FAIL\n"); return 1; }
    std::printf("009f7250_TEST PASS\n");
    return 0;
}