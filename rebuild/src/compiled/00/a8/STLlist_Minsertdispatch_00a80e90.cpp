// std::list::_M_insert_dispatch(iterator pos, const_iterator first, const_iterator last, const __false_type&) -- STLport range insert for a list of counted region-connection pointers: copy every pointer of [first, last) into a new node (count bumped) linked in before pos
// Leaf: ??$_M_insert_dispatch@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A80E90, 78 bytes (+ int3 padding to 80), thiscall `ret 10h` (this never read: the
//   node allocator is malloc). pos, first and last are one-word iterators by value, then the __false_type reference.
// Callers: list<CCountedPointer<CNavRegionConnection> >::insert(pos, first, last) 0x00A81B80, the list copy ctor 0x00A82590,
//   the CNavRegion copy (0x00A83100 area: RegionID +0, this list +4, MostRecentSearchID +8, as in the CNavRegion ctor
//   0x00A78680) and the CNavQuadTree region build at 0x00A7A3B4.
// Identity: element = CNavRegion::Connections (Ego_r.pdb list<CCountedPointer<CNavRegionConnection> >); 0x10-byte node =
//   both links + the 8-byte counted pointer. Catalog "List_InsertCopy_Range" is descriptive.
// Shape: STLport 4.6 `for (; first != last; ++first) insert(pos, *first);` with insert and _M_create_node inline
//   (allocate(1) = malloc(0x10), `_Construct(&p->_M_data, x)` = placement new with the null test, CCountedPointer copy:
//   both words, `test ; je ; inc [count]`), then the link with the neighbours cached. No /EHsc: no unwind.
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14
inline void* __cdecl operator new(unsigned int, void* __p) { return __p; }   // placement new (null test on the target)

struct SCountBlock {
    long m_references;                                      // +0
    void (__fastcall* m_pfnDestroy)(void*);                 // +4
    void* m_pObject;                                        // +8
};

template <class T> class CCountedPointer {
public:
    T*           m_pObject;                                 // +0
    SCountBlock* m_pCount;                                  // +4
    CCountedPointer(const CCountedPointer<T>& other) : m_pObject(other.m_pObject), m_pCount(other.m_pCount)
    {
        if (m_pCount) ++m_pCount->m_references;
    }
};

class CNavRegionConnection;

namespace std {
class __false_type { public: __false_type() { } };

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator<_Tp>&) { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};

struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;
};

struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* __x) : _M_node(__x) { }
    void _M_incr() { _M_node = _M_node->_M_next; }
    bool operator==(const _List_iterator_base& __y) const { return _M_node == __y._M_node; }
    bool operator!=(const _List_iterator_base& __y) const { return _M_node != __y._M_node; }
};

template <class _Tp> struct _Nonconst_traits { typedef _Tp& reference; };
template <class _Tp> struct _Const_traits { typedef const _Tp& reference; };

template <class _Tp, class _Traits>
struct _List_iterator : public _List_iterator_base {
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    typedef _List_iterator<_Tp, _Traits> _Self;
    typedef _List_node<_Tp> _Node;
    _List_iterator(_Node* __x) : _List_iterator_base(__x) { }
    _List_iterator(const iterator& __x) : _List_iterator_base(__x._M_node) { }   // the copy ctor of `iterator`
    typename _Traits::reference operator*() const { return ((_Node*)_M_node)->_M_data; }
    _Self& operator++() { this->_M_incr(); return *this; }
};

template <class _Tp, class _Alloc>
class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    _STLP_alloc_proxy<_List_node_base*, _Node, allocator<_Node> > _M_node;
    _List_base(const _Alloc& __a) : _M_node(allocator<_Node>(), (_List_node_base*)0)
    {
        _Node* __n = _M_node.allocate(1);
        __n->_M_next = __n;
        __n->_M_prev = __n;
        _M_node._M_data = __n;
    }
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class list : public _List_base<_Tp, _Alloc> {
public:
    typedef _List_node<_Tp> _Node;
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    typedef _List_iterator<_Tp, _Const_traits<_Tp> >    const_iterator;

    _Node* _M_create_node(const _Tp& __x)
    {
        _Node* __p = this->_M_node.allocate(1);
        _Construct(&__p->_M_data, __x);
        return __p;
    }
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
    template <class _InputIter>
    void _M_insert_dispatch(iterator __position, _InputIter __first, _InputIter __last, const __false_type&)
    {
        for ( ; __first != __last; ++__first)
            insert(__position, *__first);
    }
};
}

typedef CCountedPointer<CNavRegionConnection> CConnectionPtr;
typedef std::list<CConnectionPtr>            CConnectionList;

template void CConnectionList::_M_insert_dispatch<CConnectionList::const_iterator>(CConnectionList::iterator, CConnectionList::const_iterator, CConnectionList::const_iterator, const std::__false_type&);