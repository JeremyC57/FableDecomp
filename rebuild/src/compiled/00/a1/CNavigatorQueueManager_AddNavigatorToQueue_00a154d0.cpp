// CNavigatorQueueManager::AddNavigatorToQueue(const CNavigatorBase& navigator, bool immediately, float priority) -- queue a navigator for route calculation, at the front or ordered by priority
// Leaf: ?AddNavigatorToQueue@CNavigatorQueueManager@@QAEXABVCNavigatorBase@@_NM@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A154D0, 239 bytes, `ret 0Ch`. Only caller 0x00A1D0C2. PDB name (Ego_r).
// Callees: malloc 0x00BFEA0E (STLport node allocator inlined).
// Shape: STLport 4.6 list<pair<long, const CNavigatorBase*> > with distance()-based size(), advance() and the
//   inline insert(); `immediately` pushes rank -1 at the front, otherwise rank = (long)(35 / max(priority, 1)) and
//   the entry goes after the first two queued navigators, past every entry ranked >= rank or -1.
extern "C" void* __cdecl malloc(unsigned int size);    // 0x00BFEA0E
inline void* operator new(unsigned int, void* where) { return where; }

class CNavigatorBase;

namespace std {
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
    _Tp* allocate(unsigned int __n) { return (_Tp*)malloc(__n * sizeof(_Tp)); }
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
    typedef std::pair<long, const CNavigatorBase*> CQueueEntry;
    std::list<CQueueEntry> Navigators;                 // +0x00 (STLport: one header pointer)
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
    std::list<CQueueEntry>::iterator it = Navigators.begin();
    std::advance(it, 2);
    while (it != Navigators.end() && (it->first >= rank || it->first == -1))
        ++it;
    Navigators.insert(it, CQueueEntry(rank, &navigator));
}