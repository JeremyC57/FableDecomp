// std::_Deque_base<C2DVector, std::allocator<C2DVector> >::~_Deque_base() -- when the map exists: free the node buffers
//   [start node, finish node] (_M_destroy_nodes 0x0048C820), then the map itself
// Leaf: ??1?$_Deque_base@VC2DVector@@V?$allocator@VC2DVector@@@std@@@std@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0048CAF0, 46 bytes, thiscall `ret`: test [this+20h]; push finish._M_node + 4,
//   start._M_node; call 0x0048C820; reread [this+20h], free it when non-null.
// Identity (sweep3; catalog CBankFileAsyncData::FreeBuffer, a `struct T` stand-in calling an invented Release_0048c820):
//   the body is STLport 4.6 ~_Deque_base over the deque<C2DVector> layout (map +0x20, start._M_node +0x0C,
//   finish._M_node +0x1C) and its callee is _M_destroy_nodes 0x0048C820 (sweep3 RENAMES).
//   deque<C2DVector> is the element type: the landed std::deque<C2DVector>::erase 0x00A0F040 (chunk 483) calls 0x0048C820
//   twice (`_M_destroy_nodes(_M_start._M_node, new_start._M_node)` / `(new_finish._M_node + 1, _M_finish._M_node + 1)`),
//   no ICF in retail so one instantiation; Ego_r/FableWin hold deque<C2DVector> (Dinkumware member names there; retail
//   is STLport 4.6). STLport 4.6 _deque.h (and libstdc++ 13 stl_deque.h, the same SGI code) declare _M_destroy_nodes
//   as a protected member of _Deque_base (`class deque : protected _Deque_base`), and ~_Deque_base as
//   `if (_M_map._M_data) { _M_destroy_nodes(_M_start._M_node, _M_finish._M_node + 1); _M_map.deallocate(_M_map._M_data,
//   _M_map_size._M_data); }`; the allocator's deallocate is `if (p) free(p)` (MSVCR71 free 0x00BFEA14).
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
template <class _Tp, class _Traits> struct _Deque_iterator : public _Deque_iterator_base<_Tp> { };

template <class _Tp, class _Alloc> class _Deque_base {      // STLport 4.6 _deque.h
public:
    typedef _Deque_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    static unsigned int buffer_size() { return (unsigned int)_Deque_iterator_base<_Tp>::__buffer_size; }
    ~_Deque_base();
protected:
    void _M_destroy_nodes(_Tp** __nstart, _Tp** __nfinish);
    iterator _M_start;                                      // +0x00
    iterator _M_finish;                                     // +0x10
    _STLP_alloc_proxy<_Tp**, _Tp*, allocator<_Tp*> > _M_map;            // +0x20
    _STLP_alloc_proxy<unsigned int, _Tp, _Alloc>     _M_map_size;       // +0x24
};

template <class _Tp, class _Alloc>
_Deque_base<_Tp, _Alloc>::~_Deque_base()
{
    if (_M_map._M_data) {
        _M_destroy_nodes(_M_start._M_node, _M_finish._M_node + 1);
        _M_map.deallocate(_M_map._M_data, _M_map_size._M_data);
    }
}

template class _Deque_base<C2DVector, allocator<C2DVector> >;
}
