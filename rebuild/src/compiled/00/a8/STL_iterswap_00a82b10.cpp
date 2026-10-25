// std::iter_swap(SSwitchableLineList* a, SSwitchableLineList* b) -- swap two 16-byte (unsigned __int64 ID, list of lines) switchable line-list entries through a by-value temporary (key words + a fresh copy of the list), then destroy the temporary
// Leaf: ??$iter_swap@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A82B10, 138 bytes, plain `ret` (/Gr: a in ecx, b in edx).
// Callees: malloc 0x00BFEA0E (the temporary's list sentinel), list<C2DLineF>::_M_insert_dispatch 0x005152C0 (landed),
//   list<C2DLineF>::operator= 0x00517720 (twice), the list destructor 0x005127A0 (the temporary).
// Identity: element = pair<unsigned __int64, list<C2DLineF> > (CNavigationLayer::SwitchableLineLists). Callers: the
//   __unguarded_partition at 0x00A83CFC (`iter_swap(first, last)`) and a tail jump at 0x00A834A0. Catalog
//   "GuiTree_SwapAndReorder" is propagated (wrong).
// Shape: STLport 4.6 -- `iter_swap(i1, i2)` -> inline `__iter_swap(a, b, value_type)`: `_Tp tmp = *a; *a = *b; *b = tmp;`
//   (pair copy ctor inline: key words into the frame, `_List_base(a)` sentinel, then the out-of-line range insert with
//   `pos` built in its slot; the pair's implicit assignment twice: key words + the list's operator=), then ~tmp.
//   __iter_swap must be __forceinline (plain `inline` stays an out-of-line cdecl call, 13 v 138), and the list's
//   operator=, destructor and range insert need same-TU noinline bodies: only then does VC7.1 treat tmp as
//   uncaptured -- the dead proxy zero store before the malloc disappears and the key's low word stays in ebp across
//   the calls (declaration-only: 144 v 138).
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
    void _M_erase(_List_node_base* __first, _List_node_base* __last);   // 0x00512120 erase(first, last)
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

#pragma auto_inline(off)
namespace std {
// 0x005152C0 (landed): the real range insert as a same-TU noinline body.
template <class _Tp, class _Alloc> template <class _InputIter>
__declspec(noinline) void list<_Tp, _Alloc>::_M_insert_dispatch(iterator __pos, _InputIter __first, _InputIter __last, const __false_type&)
{
    for (; __first._M_node != __last._M_node; __first._M_node = __first._M_node->_M_next) {
        _List_node<_Tp>* __tmp = (_List_node<_Tp>*)malloc(sizeof(_List_node<_Tp>));
        new (&__tmp->_M_data) _Tp(((_List_node<_Tp>*)__first._M_node)->_M_data);
        _List_node_base* __n = __pos._M_node;
        _List_node_base* __p = __n->_M_prev;
        __tmp->_M_next = __n;
        __tmp->_M_prev = __p;
        __p->_M_next = __tmp;
        __n->_M_prev = __tmp;
    }
}
// 0x005127A0: the list destructor (clear, then free the sentinel) as a same-TU noinline body.
template <class _Tp, class _Alloc>
__declspec(noinline) _List_base<_Tp, _Alloc>::~_List_base()
{
    _List_node_base* __cur = _M_node._M_data->_M_next;
    while (__cur != _M_node._M_data) {
        _List_node_base* __tmp = __cur;
        __cur = __cur->_M_next;
        free(__tmp);
    }
    _M_node._M_data->_M_next = _M_node._M_data;
    _M_node._M_data->_M_prev = _M_node._M_data;
    if (_M_node._M_data != 0)
        free(_M_node._M_data);
}
// 0x00517720: list::operator= (STLport: assign the common prefix element-wise, then erase the surplus or insert the
// rest) as a same-TU noinline body; the surplus erase 0x00512120 stays external.
template <class _Tp, class _Alloc>
__declspec(noinline) list<_Tp, _Alloc>& list<_Tp, _Alloc>::operator=(const list<_Tp, _Alloc>& __x)
{
    if (this != &__x) {
        _List_node_base* __first1 = this->_M_node._M_data->_M_next;
        _List_node_base* __last1 = this->_M_node._M_data;
        _List_node_base* __first2 = __x._M_node._M_data->_M_next;
        _List_node_base* __last2 = __x._M_node._M_data;
        while (__first1 != __last1 && __first2 != __last2) {
            ((_List_node<_Tp>*)__first1)->_M_data = ((_List_node<_Tp>*)__first2)->_M_data;
            __first1 = __first1->_M_next;
            __first2 = __first2->_M_next;
        }
        if (__first2 == __last2)
            _M_erase(__first1, __last1);
        else
            _M_insert_dispatch(iterator(__last1), const_iterator(__first2), const_iterator(__last2), __false_type());
    }
    return *this;
}
}
#pragma auto_inline(on)

namespace std {
template <class _Tp> inline _Tp* __value_type_of(_Tp*) { return (_Tp*)0; }

template <class _ForwardIter1, class _ForwardIter2, class _Tp>
__forceinline void __iter_swap(_ForwardIter1 __a, _ForwardIter2 __b, _Tp*)
{
    _Tp __tmp = *__a;
    *__a = *__b;
    *__b = __tmp;
}
template <class _ForwardIter1, class _ForwardIter2>
void __fastcall iter_swap(_ForwardIter1 __i1, _ForwardIter2 __i2)
{
    __iter_swap(__i1, __i2, __value_type_of(__i1));
}
template void __fastcall iter_swap(SSwitchableLineList*, SSwitchableLineList*);
}