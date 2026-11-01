// std::vector::_M_clear() -- (vector<CTVertexLandscapeBackground>) STLport 4.6 `_M_clear()`: the trivial element destroy, then free the block when non-null
// Leaf: ?_M_clear@?$vector@VCTVertexLandscapeBackground@@V?$allocator@VCTVertexLandscapeBackground@@@std@@@std@@IAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B6D250, 14 bytes (size-optimised: pop ecx).
// Identity (naming wave 20, chunk 688): the `_M_clear()` the landed vector<CTVertexLandscapeBackground>::_M_insert_overflow 0x00B6D0B0 calls
//   (itself the callee of the chunk 688 _M_fill_insert 0x00B6CE00, proven through the PDB resize 0x00B6CD10) in the 0x00B6CD10..0x00B6D250
//   vector<CTVertexLandscapeBackground> COMDAT run; the element's destructor is trivial, so only the block free remains. Was the descriptive
//   _unattributed::ReleaseFirstMember_00b6d250 (renamed in the fistp pass from the propagated CFileInstaller::CActiveFile::OnReadFinished).
#pragma optimize("s", on)
extern "C" void __cdecl free(void* block);                   // 0x00BFEA14 (MSVCR71)

class CTVertexLandscapeBackground {                // Ego_r.pdb, 24 bytes (CTVertexLandscapeBackgroundBase layout; the chunk 688 _M_fill_insert 0x00B6CE00 model)
public:
    unsigned short X;                              // +0x00
    unsigned short Y;                              // +0x02
    float Z;                                       // +0x04
    float NormalX;                                 // +0x08
    float NormalY;                                 // +0x0C
    float NormalZ;                                 // +0x10
    unsigned char UVPad1;                          // +0x14
    unsigned char V1;                              // +0x15
    unsigned char U1;                              // +0x16
    unsigned char UVPad2;                          // +0x17
};

namespace std {
struct __true_type { };
struct __false_type { };
template <class _Tp> struct __type_traits { typedef __false_type has_trivial_destructor; };
template <> struct __type_traits<CTVertexLandscapeBackground> { typedef __true_type has_trivial_destructor; };

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
    _Tp* _M_start;                                              // +0x00
    _Tp* _M_finish;                                             // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;     // +0x08
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
protected:
    void _M_clear()
    {
        _Destroy(this->_M_start, this->_M_finish);
        this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
    }
};
}

template void std::vector<CTVertexLandscapeBackground, std::allocator<CTVertexLandscapeBackground> >::_M_clear();
