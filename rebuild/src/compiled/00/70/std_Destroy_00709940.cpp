// std::_Destroy(CEngineTextureDecal* __first, CEngineTextureDecal* __last) -- destroy a range of CEngineTextureDecals: each element's vector frees its block
// Leaf: ??$_Destroy@PAVCEngineTextureDecal@@@std@@YIXPAVCEngineTextureDecal@@0@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00709940, 44 bytes, `__fastcall` (first ecx, last edx), plain `ret`.
// Identity (proven): bound ??$_Destroy@PAVCEngineTextureDecal@@@std@@YIXPAVCEngineTextureDecal@@0@Z from its only caller, the landed vector<CEngineTextureDecal>::operator= 0x0077B860; 20-byte stride, CEngineTextureDecal::OverlaidBlends (Ego_r.pdb vector<CCompositeBlendDesc> at +8) freed.
// Callees: free 0x00BFEA14 (allocator::deallocate inside the member vector's destructor).
// Shape (family M188, representative 0x00675500): STLport 4.6 `_Destroy(first, last)` -> `__destroy` -> `__destroy_aux(..., __false_type())`,
//   `for (; __first != __last; ++__first) _Destroy(&*__first);` all inline (the landed 0x00CDB820 spelling) --
//   the element's implicit destructor is the member vector's (its own elements trivially destructible), and the STLport
//   _Vector_base null test `if (_M_start != 0) deallocate(...)` is what makes VC test first == last before saving esi and walk a
//   pointer to the member (+0x8), testing `esi - 8` against last (without it the loop indexes off first). Speed TU.
extern "C" void __cdecl free(void* block);                 // 0x00BFEA14
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;                                            // +0x00
    _T2 second;                                           // +0x04
};
struct __false_type { __false_type() { } };
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator>
inline void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _ForwardIterator, class _Tp>
inline void __fastcall __destroy(_ForwardIterator __first, _ForwardIterator __last, _Tp*)
{
    __destroy_aux(__first, __last, __false_type());
}
template <class _ForwardIterator>
inline void __fastcall _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    __destroy(__first, __last, __first);
}
template <class _Tp> class allocator {
public:
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    ~_Vector_base() { if (_M_start != 0) _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
    _Tp* _M_start;                                         // +0x00
    _Tp* _M_finish;                                        // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;  // +0x08
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};
}
enum ECompositeBlendType { COMPOSITE_BLEND_TYPE_NONE = 0 };
class CRGBColour { public: unsigned long Colour; };
class CCompositeBlendDesc {                                // Ego_r.pdb, 16 bytes
public:
    unsigned long TextureIndex;                           // +0x00
    ECompositeBlendType BlendType;                        // +0x04
    CRGBColour DiffuseColour;                             // +0x08
    CRGBColour SpecularColour;                            // +0x0C
};
class CEngineTextureDecal {                                // Ego_r.pdb, 20 bytes in retail
public:
    long OriginalTextureIndex;                            // +0x00
    long DecalID;                                         // +0x04
    std::vector<CCompositeBlendDesc> OverlaidBlends;      // +0x08
};
namespace std {
template void __fastcall _Destroy<CEngineTextureDecal*>(CEngineTextureDecal* __first, CEngineTextureDecal* __last);
}