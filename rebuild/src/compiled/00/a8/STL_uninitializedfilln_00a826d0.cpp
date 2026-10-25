// std::__uninitialized_fill_n(CLineList* first, unsigned int n, const CLineList& x, const __false_type&) -- copy-construct n lists of lines from x into raw storage (each gets its own sentinel, then a copy of every line), return the end
// Leaf: ??$__uninitialized_fill_n@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A826D0, 83 bytes (+ int3 padding to 96), `ret 8` (/Gr: first in ecx, n in edx;
//   x and the tag reference on the stack).
// Callees: malloc 0x00BFEA0E (the 0x18-byte sentinel), list<C2DLineF>::_M_insert_dispatch 0x005152C0 (landed).
// Identity: element = list<C2DLineF> -- CNavigationLayer::LineLists (Ego_r.pdb vector<list<C2DLineF> >), 4 bytes in
//   retail STLport. Caller: the vector's fill insert / resize of this TU. Catalog `_Fill_n<unsigned_long*,...>` is
//   propagated (wrong element and helper).
// Shape: STLport 4.6 verbatim -- `for (; n > 0; --n, ++cur) _Construct(&*cur, x); return cur;` (unsigned n: `test ;
//   jbe`; placement-new null test) with list's copy ctor inline (`_List_base(a)`: proxy zeroed, sentinel malloc'd and
//   self-linked, then `insert(begin(), x.begin(), x.end())` out of line; the __false_type temporary homed in the dead
//   x argument slot, x itself kept in edi).
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14
inline void* __cdecl operator new(unsigned int, void* __p) { return __p; }   // placement new (null test on the target)

class C2DVector {
public:
    float X;                                                // +0
    float Y;                                                // +4
};
class C2DLineF {                                            // 16 bytes
public:
    C2DVector Start;                                        // +0x0
    C2DVector End;                                          // +0x8
};

namespace std {
class __false_type { public: __false_type() { } };

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
};

template <class _Tp> struct _Nonconst_traits { typedef _Tp& reference; };
template <class _Tp> struct _Const_traits { typedef const _Tp& reference; };

template <class _Tp, class _Traits>
struct _List_iterator : public _List_iterator_base {
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    typedef _List_node<_Tp> _Node;
    _List_iterator(_List_node_base* __x) : _List_iterator_base(__x) { }
    _List_iterator(const iterator& __x) : _List_iterator_base(__x._M_node) { }   // the copy ctor of `iterator`
};

template <class _Tp, class _Alloc>
class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    typedef allocator<_Tp> allocator_type;
    _STLP_alloc_proxy<_List_node_base*, _Node, allocator<_Node> > _M_node;
    _List_base(const allocator_type&) : _M_node(allocator<_Node>(), (_List_node_base*)0)
    {
        _Node* __n = _M_node.allocate(1);
        __n->_M_next = __n;
        __n->_M_prev = __n;
        _M_node._M_data = __n;
    }
    ~_List_base();                                          // 0x005127A0 (clear, then free the sentinel)
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class list : public _List_base<_Tp, _Alloc> {
public:
    typedef _List_base<_Tp, _Alloc> _Base;
    typedef typename _Base::allocator_type allocator_type;
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    typedef _List_iterator<_Tp, _Const_traits<_Tp> >    const_iterator;
    allocator_type get_allocator() const { return allocator_type(); }
    iterator begin() { return iterator(this->_M_node._M_data->_M_next); }
    const_iterator begin() const { return const_iterator(this->_M_node._M_data->_M_next); }
    const_iterator end() const { return const_iterator(this->_M_node._M_data); }
    template <class _InputIter>
    void _M_insert_dispatch(iterator __pos, _InputIter __first, _InputIter __last, const __false_type&);   // 0x005152C0
    template <class _InputIter>
    void insert(iterator __pos, _InputIter __first, _InputIter __last) { _M_insert_dispatch(__pos, __first, __last, __false_type()); }
    list(const list<_Tp, _Alloc>& __x) : _List_base<_Tp, _Alloc>(__x.get_allocator()) { insert(begin(), __x.begin(), __x.end()); }
    list<_Tp, _Alloc>& operator=(const list<_Tp, _Alloc>& __x);   // 0x00517720
};

template <class _T1, class _T2> class pair {
public:
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};
}

template <class K, class V> class CKeyPairCompareLess {
public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};

typedef std::list<C2DLineF>                                    CLineList;            // 4 bytes (0x18-byte nodes)
typedef std::pair<unsigned __int64, CLineList>                 SSwitchableLineList;  // 16 bytes
typedef CKeyPairCompareLess<unsigned __int64, CLineList>        CSwitchableLineListLess;

namespace std {
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}
template CLineList* __fastcall __uninitialized_fill_n(CLineList*, unsigned int, const CLineList&, const __false_type&);
}