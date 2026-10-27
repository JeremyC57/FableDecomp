// std::deque<C2DVector>::~deque() -- STLport deque destructor: destroy every element, then free the node buffers and the map
// Leaf: ??1?$deque@VC2DVector@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0048D520, 150 bytes, thiscall, plain `ret`.
// Identity (proven): its _M_destroy_nodes 0x0048C820 is the landed _Deque_base<C2DVector>::_M_destroy_nodes and its destroy loop 0x0048D050 is bound ??$__destroy_aux@U?$_Deque_iterator@VC2DVector@@...; caller CInputProcessCombat's ??_G 0x0048D680.
// Callees: __destroy_aux(first, last, const __false_type&) 0x0048D050 (__fastcall: the two 16-byte iterators by value, the tag
//   in ecx), _Deque_base::_M_destroy_nodes 0x0048C820, free 0x00BFEA14.
// Shape (family K007, representative 0x004427D0): STLport 4.6 `~deque() { _Destroy(_M_start, _M_finish); }` with the inline
//   ~_Deque_base (the landed 0x0048CAF0 body).
extern "C" void __cdecl free(void* block);                 // 0x00BFEA14
class C2DVector {                                           // 8 bytes; a class in Ego_r/FableWin (V key)
public:
    float X, Y;
};
namespace std {
template <class _Tp> class allocator {
public:
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp> struct _Nonconst_traits { typedef _Tp& reference; typedef _Tp* pointer; };

template <class _Tp> struct _Deque_iterator_base {          // 16 bytes
    enum _Constants { _blocksize = 128, __buffer_size = (sizeof(_Tp) < (unsigned int)_blocksize ? ((unsigned int)_blocksize / sizeof(_Tp)) : 1) };
    _Tp*  _M_cur;                                           // +0x00
    _Tp*  _M_first;                                         // +0x04
    _Tp*  _M_last;                                          // +0x08
    _Tp** _M_node;                                          // +0x0c
};
template <class _Tp, class _Traits> struct _Deque_iterator : public _Deque_iterator_base<_Tp> {
    typedef _Deque_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    _Deque_iterator(const iterator& __x) : _Deque_iterator_base<_Tp>(__x) { }   // STLport 4.6: the user copy constructor
};

template <class _Tp, class _Alloc> class _Deque_base {      // STLport 4.6 _deque.h
public:
    typedef _Deque_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    static unsigned int buffer_size() { return (unsigned int)_Deque_iterator_base<_Tp>::__buffer_size; }
    __forceinline ~_Deque_base();
protected:
    void _M_destroy_nodes(_Tp** __nstart, _Tp** __nfinish);   // 0x0048C820
    iterator _M_start;                                      // +0x00
    iterator _M_finish;                                     // +0x10
    _STLP_alloc_proxy<_Tp**, _Tp*, allocator<_Tp*> > _M_map;            // +0x20
    _STLP_alloc_proxy<unsigned int, _Tp, _Alloc>     _M_map_size;       // +0x24
};

template <class _Tp, class _Alloc>
__forceinline _Deque_base<_Tp, _Alloc>::~_Deque_base()
{
    if (_M_map._M_data) {
        _M_destroy_nodes(_M_start._M_node, _M_finish._M_node + 1);
        _M_map.deallocate(_M_map._M_data, _M_map_size._M_data);
    }
}
struct __false_type { __false_type() { } };
template <class _Tp> struct __type_traits { typedef __false_type has_trivial_destructor; };
template <class _ForwardIterator>
void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&);   // 0x0048D050
template <class _Tp, class _ForwardIterator>
__forceinline void _Destroy_range(_ForwardIterator __first, _ForwardIterator __last, _Tp*)   // STLport _Destroy(first, last): iterators by value
{
    typedef typename __type_traits<_Tp>::has_trivial_destructor _Trivial_destructor;
    __destroy_aux(__first, __last, _Trivial_destructor());
}
template <class _Tp, class _Alloc = allocator<_Tp> > class deque : protected _Deque_base<_Tp, _Alloc> {
public:
    ~deque() { _Destroy_range(this->_M_start, this->_M_finish, (_Tp*)0); }
};
template deque<C2DVector >::~deque();
}