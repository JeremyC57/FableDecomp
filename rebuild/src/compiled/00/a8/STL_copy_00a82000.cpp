// _STL::__copy(SSwitchableLineList* first, SSwitchableLineList* last, SSwitchableLineList* result, const random_access_iterator_tag&, int*) -- STLport random-access copy of the 16-byte (unsigned __int64 ID, list of lines) switchable line-list entries: counted forward assignment, returns the advanced result
// Leaf: ??$__copy@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A82000, 67 bytes (+ int3 padding to 80), `ret 0Ch` (/Gr: first in ecx, last in
//   edx; result, the tag reference and the null distance pointer on the stack).
// Callees: list<C2DLineF>::operator= 0x00517720.
// Identity: element = pair<unsigned __int64, list<C2DLineF> > (CNavigationLayer::SwitchableLineLists, Ego_r.pdb
//   CVectorMap<unsigned __int64, list<C2DLineF> >); the 16-byte stride gives `sar edx,4`. Catalog
//   `_Move<CEngineTextureMorph*,CEngineTextureMorph*>` is propagated (wrong element).
// Shape: STLport 4.6 verbatim -- `for (_Distance n = last - first; n > 0; --n) { *result = *first; ++first; ++result; }
//   return result;`, the pair's implicit assignment inline (two key words, then the list's out-of-line operator=).
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

namespace _STL {
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
    bool operator()(const _STL::pair<K, V>& a, const _STL::pair<K, V>& b) const { return a.first < b.first; }
};

typedef _STL::list<C2DLineF>                                    CLineList;            // 4 bytes (0x18-byte nodes)
typedef _STL::pair<unsigned __int64, CLineList>                 SSwitchableLineList;  // 16 bytes
typedef CKeyPairCompareLess<unsigned __int64, CLineList>        CSwitchableLineListLess;

namespace _STL {
class random_access_iterator_tag { public: random_access_iterator_tag() { } };

template <class _RandomAccessIter, class _OutputIter, class _Distance>
_OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                              const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}
template SSwitchableLineList* __fastcall __copy(SSwitchableLineList*, SSwitchableLineList*, SSwitchableLineList*, const random_access_iterator_tag&, int*);
}