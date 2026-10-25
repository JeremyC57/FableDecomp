// std::vector::~vector() -- (vector<CGPUScratchBufferTextureManager::CResourcePage >) out-of-line destructor: destroy every 0x1C-byte element (its implicit destructor frees the inner ResourceList array at +0x4 when it has a block), then free the vector's own block
// Leaf: ??1?$vector@UCResourcePage@CGPUScratchBufferTextureManager@@V?$allocator@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F386D0, 58 bytes (+ int3 padding), thiscall, plain `ret`.
// Identity: vector<CGPUScratchBufferTextureManager::CResourcePage >::~vector -- probable; evidence: only the unreferenced jmp thunk 0x00F38760 (the CArray-level destructor) reaches it, in the CResourcePage vector TU: the landed _M_clear 0x00F38710 of vector<CGPUScratchBufferTextureManager::CResourcePage> beside it has the same bytes (0x1C-byte pages, ResourceList's block at +4); FableWin.pdb spells CResourcePage a struct (U).
// Label: the manifest's "sub_00f386d0" is descriptive / propagated across byte-identical bodies.
// Callees: free 0x00BFEA14 (twice: the element's inner block, then the vector's block).
// Body: STLport 4.6 `~vector() { _Destroy(_M_start, _M_finish); }` with the inline `~_Vector_base() { if (_M_start) deallocate(_M_start, ...); }`; the element destructors are implicit (the inner vector's
//   trivially destructible elements need no loop: STLport's __type_traits<T>::has_trivial_destructor picks the empty
//   __destroy_aux).
extern "C" void __cdecl free(void* block);                   // 0x00BFEA14
namespace std {
struct __true_type { };
struct __false_type { };
template <class _Tp> struct __type_traits { typedef __false_type has_trivial_destructor; };

template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for ( ; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator, _ForwardIterator, const __true_type&) { }
template <class _Tp> inline void _Destroy(_Tp* __first, _Tp* __last)
{
    typedef typename __type_traits<_Tp>::has_trivial_destructor _Trivial_destructor;
    __destroy_aux(__first, __last, _Trivial_destructor());
}
template <class _Tp> class allocator {
public:
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc> class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base() : _M_start(0), _M_finish(0) { _M_end_of_storage._M_data = 0; }
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
    _Tp* _M_start;                                              // +0x00
    _Tp* _M_finish;                                             // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;     // +0x08
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
    void _M_clear()                                             // protected in STLport 4.6; public as the landed bindings spell it
    {
        _Destroy(this->_M_start, this->_M_finish);
        this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
    }
};
}
template <class T> class CArray : public std::vector<T> { };     // retail CArray: an STLport vector (12 bytes)
class CGPUScratchBufferTextureManager {
public:
    struct CResourcePage {                                    // 28 bytes (Ego_r.pdb 32 with its 16-byte CArray)
        long           ScratchPool;                           // +0x00
        CArray<long>   ResourceList;                          // +0x04
        unsigned char* NextFreeBufferAddress;                 // +0x10
        long           FreeSpaceSize;                         // +0x14
        bool           EnableAutoRestore;                     // +0x18
    };
};
namespace std {
template <> struct __type_traits<long > { typedef __true_type has_trivial_destructor; };
}
template class std::vector<CGPUScratchBufferTextureManager::CResourcePage >;