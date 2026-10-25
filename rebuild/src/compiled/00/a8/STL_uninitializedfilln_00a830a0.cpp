// _STL::__uninitialized_fill_n(pair<unsigned __int64, list<C2DLineF> >* first, unsigned int n, const pair<unsigned __int64, list<C2DLineF> >& x, const __false_type&) -- copy-construct n (UID, line list) records into raw storage at first and return first + n
// Leaf: ??$__uninitialized_fill_n@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A830A0, 109 bytes (+3 int3 padding), __fastcall (first ecx, n edx; x and the empty tag on the stack, `ret 8`).
// Callers: one, 0x00A84C9A -- the vector's _M_insert_overflow 0x00A84C20 (landed) in the navigation quad-tree TU; the byte twin
//   0x0051A880 is the same instantiation in the CNavigationLayer TU.
// Callees: malloc 0x00BFEA0E (the list's 0x18-byte header node), list<C2DLineF>::_M_insert_dispatch(iterator,
//   const_iterator, const_iterator, const __false_type&) 0x005152C0.
// Element: the 16-byte (unsigned __int64 key, list<C2DLineF>) record of Ego_r.pdb CNavigationLayer's CVectorMap (retail
//   STLport list = one header-node pointer, node 0x18 bytes: next, prev, the 16-byte C2DLineF).
// Label: catalog "vector<unsigned __int64>::_Ufill" is propagated across the byte-identical-shaped STLport helpers.
// Shape: STLport 4.6 __uninitialized_fill_n verbatim (unsigned n so `jbe`, the count spilled to the `push ecx` frame word)
//   with the pair's memberwise copy inline: the key as two dwords, then list's copy constructor -- `_List_base(a)` nulls
//   the node word before the malloc and self-links the header, then `insert(begin(), x.begin(), x.end())` forwards to the
//   out-of-line _M_insert_dispatch: the iterator has a user copy constructor so `pos` is built in its argument slot, the
//   const_iterators are pushed as plain words and the empty __false_type temporary is homed in the dead `x` slot (as the
//   landed __uninitialized_copy 0x00A82730 of the same list).
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E
inline void* operator new(unsigned int, void* p) { return p; }

class C2DVector {
public:
    float X;                                    // +0x0
    float Y;                                    // +0x4
};
class C2DLineF {
public:
    C2DVector Point1;                           // +0x0
    C2DVector Point2;                           // +0x8
};

namespace _STL {
struct __false_type { __false_type() { } };

struct _List_node_base {
    _List_node_base* _M_next;                   // +0x0
    _List_node_base* _M_prev;                   // +0x4
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;                                // +0x8
};

struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* __x) : _M_node(__x) { }
};
template <class _Tp> struct _List_iterator : public _List_iterator_base {          // iterator: user copy constructor
    _List_iterator(_List_node_base* __x) : _List_iterator_base(__x) { }
    _List_iterator(const _List_iterator<_Tp>& __x) : _List_iterator_base(__x._M_node) { }
};
template <class _Tp> struct _List_const_iterator : public _List_iterator_base {    // const_iterator: implicit copy
    _List_const_iterator(_List_node_base* __x) : _List_iterator_base(__x) { }
};

template <class _Tp> class allocator {
public:
    allocator() { }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};

template <class _Tp, class _Alloc> class _List_base {
public:
    _STLP_alloc_proxy<_List_node<_Tp>*, _List_node<_Tp>, allocator<_List_node<_Tp> > > _M_node;   // +0x0 header
    _List_base(const allocator<_List_node<_Tp> >& __a) : _M_node(__a, (_List_node<_Tp>*)0)
    {
        _List_node<_Tp>* __n = (_List_node<_Tp>*)malloc(sizeof(_List_node<_Tp>));
        __n->_M_next = __n;
        __n->_M_prev = __n;
        _M_node._M_data = __n;
    }
};

template <class _Tp, class _Alloc = allocator<_Tp> > class list : public _List_base<_Tp, _Alloc> {
public:
    typedef _List_iterator<_Tp> iterator;
    typedef _List_const_iterator<_Tp> const_iterator;
    allocator<_List_node<_Tp> > get_allocator() const { return allocator<_List_node<_Tp> >(); }
    iterator begin() { return iterator(this->_M_node._M_data->_M_next); }
    const_iterator begin() const { return const_iterator(this->_M_node._M_data->_M_next); }
    const_iterator end() const { return const_iterator(this->_M_node._M_data); }
    void _M_insert_dispatch(iterator __pos, const_iterator __first, const_iterator __last, const __false_type&);   // 0x005152C0
    void insert(iterator __pos, const_iterator __first, const_iterator __last)
    {
        _M_insert_dispatch(__pos, __first, __last, __false_type());
    }
    list(const list<_Tp, _Alloc>& __x) : _List_base<_Tp, _Alloc>(__x.get_allocator())
    {
        insert(begin(), __x.begin(), __x.end());
    }
};

template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _ForwardIter, class _Size, class _Tp>
inline _ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}

typedef pair<unsigned __int64, list<C2DLineF> > CLineListRecord;      // 16 bytes

template CLineListRecord* __fastcall __uninitialized_fill_n(CLineListRecord*, unsigned int, const CLineListRecord&,
                                                            const __false_type&);
}