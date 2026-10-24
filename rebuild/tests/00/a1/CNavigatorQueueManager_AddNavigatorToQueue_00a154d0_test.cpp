// Behaviour test for CNavigatorQueueManager::AddNavigatorToQueue (0x00A154D0): `immediately` puts the navigator at
// the front with rank -1; otherwise rank = (long)(35 / max(priority, 1)); with two or fewer queued it is appended,
// else it goes after the first two entries and past every entry ranked >= rank (or -1), before the first lower one.
#include <cstdio>
#include <cstdlib>
#include <new>

static int g_allocs = 0;

class CNavigatorBase;

namespace _STL {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) {}
    pair(const pair& __p) : first(__p.first), second(__p.second) {}
};
template <class _T1, class _T2>
inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> class allocator {
public:
    allocator() {}
    _Tp* allocate(unsigned int __n) { ++g_allocs; return (_Tp*)std::malloc(__n * sizeof(_Tp)); }
};
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;
};
template <class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc { public: _Tp _M_data; };
template <class _Tp> struct _List_iterator {
    _List_node_base* _M_node;
    _List_iterator(_List_node_base* __x) : _M_node(__x) {}
    _List_iterator(const _List_iterator& __x) : _M_node(__x._M_node) {}
    _Tp& operator*() const { return ((_List_node<_Tp>*)_M_node)->_M_data; }
    _Tp* operator->() const { return &(operator*()); }
    _List_iterator& operator++() { _M_node = _M_node->_M_next; return *this; }
    bool operator==(const _List_iterator& __y) const { return _M_node == __y._M_node; }
    bool operator!=(const _List_iterator& __y) const { return _M_node != __y._M_node; }
};
template <class _InputIterator, class _Distance>
inline void advance(_InputIterator& __i, _Distance __n)
{
    while (__n--) ++__i;
}
template <class _InputIterator>
inline int distance(const _InputIterator& __first, const _InputIterator& __last)
{
    int __n = 0;
    _InputIterator __it(__first);
    while (__it != __last) {
        ++__it;
        ++__n;
    }
    return __n;
}
template <class _Tp, class _Alloc = allocator<_Tp> > class list {
public:
    typedef _List_node<_Tp> _Node;
    typedef _List_iterator<_Tp> iterator;
    typedef unsigned int size_type;
    _STLP_alloc_proxy<_List_node_base*, allocator<_Node> > _M_node;
    _Node* _M_create_node(const _Tp& __x)
    {
        _Node* __p = _M_node.allocate(1);
        _Construct(&__p->_M_data, __x);
        return __p;
    }
    iterator begin() { return iterator(_M_node._M_data->_M_next); }
    iterator end() { return iterator(_M_node._M_data); }
    size_type size() { return distance(begin(), end()); }
    iterator insert(iterator __position, const _Tp& __x)
    {
        _Node* __tmp = _M_create_node(__x);
        _List_node_base* __n = __position._M_node;
        _List_node_base* __p = __n->_M_prev;
        __tmp->_M_next = __n;
        __tmp->_M_prev = __p;
        __p->_M_next = __tmp;
        __n->_M_prev = __tmp;
        return iterator(__tmp);
    }
    void push_front(const _Tp& __x) { insert(begin(), __x); }
    void push_back(const _Tp& __x) { insert(end(), __x); }
};
}

class CNavigatorQueueManager {
public:
    typedef _STL::pair<long, const CNavigatorBase*> CQueueEntry;
    _STL::list<CQueueEntry> Navigators;                 // +0x00 (STLport: one header pointer)
    void AddNavigatorToQueue(const CNavigatorBase& navigator, bool immediately, float priority);
};

void CNavigatorQueueManager::AddNavigatorToQueue(const CNavigatorBase& navigator, bool immediately, float priority)
{
    if (immediately) {
        Navigators.push_front(CQueueEntry(-1, &navigator));
        return;
    }
    if (priority < 1.0f)
        priority = 1.0f;
    long rank = (long)(35.0f / priority);
    if (Navigators.size() <= 2) {
        Navigators.push_back(CQueueEntry(rank, &navigator));
        return;
    }
    _STL::list<CQueueEntry>::iterator it = Navigators.begin();
    _STL::advance(it, 2);
    while (it != Navigators.end() && (it->first >= rank || it->first == -1))
        ++it;
    Navigators.insert(it, CQueueEntry(rank, &navigator));
}

class CNavigatorBase { public: int Id; };

static int check(CNavigatorQueueManager& q, const long* ranks, CNavigatorBase* const* navs, int n)
{
    _STL::list<CNavigatorQueueManager::CQueueEntry>::iterator it = q.Navigators.begin();
    for (int i = 0; i < n; ++i) {
        if (it == q.Navigators.end()) return 0;
        if (it->first != ranks[i] || it->second != navs[i]) return 0;
        ++it;
    }
    if (!(it == q.Navigators.end())) return 0;
    return (int)q.Navigators.size() == n;
}

int main()
{
    int ok = 1;
    if (sizeof(void*) == 4u && sizeof(CNavigatorQueueManager) != 4u) ok = 0;
    _STL::_List_node_base head; head._M_next = &head; head._M_prev = &head;
    CNavigatorQueueManager q; q.Navigators._M_node._M_data = &head;
    CNavigatorBase a, b, c, d, e, f, g, h;
    q.AddNavigatorToQueue(a, true, 3.0f);                  // [a-1]
    q.AddNavigatorToQueue(b, false, 0.5f);                 // clamp to 1 -> rank 35, appended
    q.AddNavigatorToQueue(c, false, 5.0f);                 // rank 7, size 2 -> appended
    { long r[] = { -1, 35, 7 }; CNavigatorBase* n[] = { &a, &b, &c }; if (!check(q, r, n, 3)) ok = 0; }
    q.AddNavigatorToQueue(d, false, 2.0f);                 // rank 17: after a,b; c(7) < 17 -> before c
    { long r[] = { -1, 35, 17, 7 }; CNavigatorBase* n[] = { &a, &b, &d, &c }; if (!check(q, r, n, 4)) ok = 0; }
    q.AddNavigatorToQueue(e, false, 10.0f);                // rank 3: past d(17), c(7) -> end
    { long r[] = { -1, 35, 17, 7, 3 }; CNavigatorBase* n[] = { &a, &b, &d, &c, &e }; if (!check(q, r, n, 5)) ok = 0; }
    q.AddNavigatorToQueue(f, true, 0.0f);                  // front
    q.AddNavigatorToQueue(g, true, 0.0f);                  // front: [g f a b d c e]
    q.AddNavigatorToQueue(h, false, 1.0f);                 // rank 35: starts at a(-1) -> skip, b(35) >= 35 skip, d(17) stop
    { long r[] = { -1, -1, -1, 35, 35, 17, 7, 3 }; CNavigatorBase* n[] = { &g, &f, &a, &b, &h, &d, &c, &e }; if (!check(q, r, n, 8)) ok = 0; }
    if (g_allocs != 8) ok = 0;

    if (!ok) { std::printf("00a154d0_TEST FAIL\n"); return 1; }
    std::printf("00a154d0_TEST PASS\n");
    return 0;
}