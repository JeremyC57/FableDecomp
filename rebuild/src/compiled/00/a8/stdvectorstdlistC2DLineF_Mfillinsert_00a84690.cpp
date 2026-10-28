// std::vector<std::list<C2DLineF>>::_M_fill_insert(iterator pos, size_type n, const value_type& x) -- STLport fill insert of n copies of one line list: in place through a local copy of x when the spare capacity holds n (shift the tail, then fill), else hand over to _M_insert_overflow
// Leaf: ?_M_fill_insert@?$vector@V?$list@VC2DLineF@@V?$allocator@VC2DLineF@@@std@@@std@@V?$allocator@V?$list@VC2DLineF@@V?$allocator@VC2DLineF@@@std@@@std@@@2@@std@@QAEXPAV?$list@VC2DLineF@@V?$allocator@VC2DLineF@@@std@@@2@IABV32@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A84690, 260 bytes, thiscall `ret 0Ch`.
// Identity (proven): called by the landed vector<list<C2DLineF> >::resize 0x00A85240 (CNavigationLayer::LineLists); its
//   overflow branch is this vector's _M_insert_overflow 0x00A84270. The landed resize binds it with an invented
//   `vector_allocator` second argument; the allocator here is std::allocator, as the landed operator= 0x00A83D70 of the
//   same vector (via CNavigationLayer::operator=) spells it.
// Callees: list<C2DLineF> copy ctor 0x00518D80 and dtor 0x005127A0 (the local x copy, out of line), __uninitialized_copy
//   0x00A831D0, __copy_backward_ptrs 0x00A83230, __uninitialized_fill_n 0x00A826D0, fill 0x00A82620 (all __fastcall),
//   _M_insert_overflow 0x00A84270.
// Shape: STLport 4.6 vector.c `_M_fill_insert` verbatim (no EH); the tag-taking helpers are same-TU noinline stand-ins
//   that never read their tags, so the tags are homed on argument slots as in retail.
extern "C" void* __cdecl malloc(unsigned int size);        // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                 // 0x00BFEA14
inline void* operator new(unsigned int, void* p) { return p; }
class C2DVector { public: float X, Y; };
class C2DLineF {                                           // 16 bytes (Ego_r.pdb)
public:
    C2DVector Start;                                       // +0x0
    C2DVector End;                                         // +0x8
};
namespace std {
struct __false_type { __false_type() { } };
template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator<_Tp>&) { }
    template <class _Tp1> allocator(const allocator<_Tp1>&) { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
    _Tp* allocate(unsigned int __n) { return (_Tp*)malloc(__n * sizeof(_Tp)); }
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
    _List_iterator_base() { }
};
template <class _Tp> struct _Nonconst_traits { typedef _Tp value_type; typedef _Tp* pointer; typedef _Tp& reference; };
template <class _Tp> struct _Const_traits { typedef _Tp value_type; typedef const _Tp* pointer; typedef const _Tp& reference; };
template <class _Tp, class _Traits>
struct _List_iterator : public _List_iterator_base {
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    typedef _List_node<_Tp> _Node;
    _List_iterator(_Node* __x) : _List_iterator_base(__x) { }
    _List_iterator() { }
    _List_iterator(const iterator& __x) : _List_iterator_base(__x._M_node) { }
};
template <class _Tp, class _Alloc>
class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    typedef allocator<_Node> _Node_allocator_type;
    typedef _STLP_alloc_proxy<_Node*, _Node, _Node_allocator_type> _AllocProxy;
    typedef _Alloc allocator_type;
    allocator_type get_allocator() const { return allocator_type((const _Node_allocator_type&)_M_node); }
    _List_base(const allocator_type& __a) : _M_node(_Node_allocator_type(__a), (_Node*)0)
    {
        _Node* __n = _M_node.allocate(1);
        __n->_M_next = __n;
        __n->_M_prev = __n;
        _M_node._M_data = __n;
    }
    _AllocProxy _M_node;
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class list : public _List_base<_Tp, _Alloc> {
public:
    typedef _List_node<_Tp> _Node;
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    typedef _List_iterator<_Tp, _Const_traits<_Tp> > const_iterator;
    iterator begin() { return iterator((_Node*)(this->_M_node._M_data->_M_next)); }
    const_iterator begin() const { return const_iterator((_Node*)(this->_M_node._M_data->_M_next)); }
    const_iterator end() const { return const_iterator(this->_M_node._M_data); }
    list(const list<_Tp, _Alloc>& __x);                   // 0x00518D80
    ~list();                                               // 0x005127A0
};
#pragma auto_inline(off)
template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{   // stand-in for 0x00A831D0 (only the target row is graded)
    for (; __first != __last; ++__first, ++__result)
        __result->_M_node._M_data = __first->_M_node._M_data;
    return __result;
}
template <class _ForwardIter, class _Size, class _Tp>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{   // stand-in for 0x00A826D0
    for (; __n > 0; --__n, ++__first)
        __first->_M_node._M_data = __x._M_node._M_data;
    return __first;
}
template <class _BidirectionalIter1, class _BidirectionalIter2>
__declspec(noinline) _BidirectionalIter2 __fastcall __copy_backward_ptrs(_BidirectionalIter1 __first, _BidirectionalIter1 __last, _BidirectionalIter2 __result, const __false_type&)
{   // stand-in for 0x00A83230
    while (__first != __last)
        (--__result)->_M_node._M_data = (--__last)->_M_node._M_data;
    return __result;
}
template <class _ForwardIter, class _Tp>
__declspec(noinline) void __fastcall fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val)
{   // stand-in for 0x00A82620
    for (; __first != __last; ++__first)
        __first->_M_node._M_data = __val._M_node._M_data;
}
#pragma auto_inline(on)

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                         // +0x00
    _Tp* _M_finish;                                        // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;  // +0x08
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* pointer;
    typedef unsigned int size_type;
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    typedef _Tp* iterator;
    void _M_fill_insert(iterator __pos, size_type __n, const _Tp& __x);
protected:
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len,
                            bool __atend = false);   // 0x00A84270
};
template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::_M_fill_insert(iterator __position, size_type __n, const _Tp& __x)
{
    if (__n != 0) {
        if (size_type(this->_M_end_of_storage._M_data - this->_M_finish) >= __n) {
            _Tp __x_copy = __x;
            const size_type __elems_after = this->_M_finish - __position;
            pointer __old_finish = this->_M_finish;
            if (__elems_after > __n) {
                __uninitialized_copy(this->_M_finish - __n, this->_M_finish, this->_M_finish, __false_type());
                this->_M_finish += __n;
                __copy_backward_ptrs(__position, __old_finish - __n, __old_finish, __false_type());
                fill(__position, __position + __n, __x_copy);
            }
            else {
                __uninitialized_fill_n(this->_M_finish, __n - __elems_after, __x_copy, __false_type());
                this->_M_finish += __n - __elems_after;
                __uninitialized_copy(__position, __old_finish, this->_M_finish, __false_type());
                this->_M_finish += __elems_after;
                fill(__position, __old_finish, __x_copy);
            }
        }
        else
            _M_insert_overflow(__position, __x, __false_type(), __n);
    }
}
}
typedef std::list<C2DLineF> CLineList;
template void std::vector<CLineList>::_M_fill_insert(CLineList*, unsigned int, const CLineList&);