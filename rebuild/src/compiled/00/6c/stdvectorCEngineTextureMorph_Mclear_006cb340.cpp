// std::vector<CEngineTextureMorph>::_M_clear() -- STLport: destroy [start, finish) (each record's Blends vector frees
// its buffer), then release the block
// Leaf: ?_M_clear@?$vector@VCEngineTextureMorph@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x006CB340, 58 bytes, thiscall `ret`: the end pointer is cached, every 20-byte record's
//   Blends._M_start (+0x08) is freed when non-null (free @ 0x00BFEA14), then `if (start) free(start)`.
// Identity (FINDINGS chunk 387; was the descriptive batch row CRetailVector_006cb340::Destroy_CRetailVector_006cb340):
//   its callers are the landed std::vector<CEngineTextureMorph>::_M_insert_overflow 0x006CB390 (which already declares
//   0x006CB340 as this vector's _M_clear) and 0x00728830; the 20-byte stride with an owned block at +0x08 is
//   CEngineTextureMorph (Ego_r.pdb / struct_layouts: OriginalTextureIndex +0, MorphID +4, vector<CCompositeBlendDesc>
//   Blends +8; 20 bytes in retail with the 12-byte STLport vector), whose implicit destructor is the inline
//   ~vector<CCompositeBlendDesc> (CCompositeBlendDesc is trivially destructible: no element loop, only the deallocate).
// Shape: STLport 4.x `_M_clear() { _Destroy(_M_start, _M_finish); _M_end_of_storage.deallocate(_M_start,
//   _M_end_of_storage._M_data - _M_start); }`, the whole _Destroy -> __destroy_aux(.., __false_type) -> ~T chain inline.
extern "C" void __cdecl free(void* block);            // 0x00BFEA14

class CRGBColour {                                   // D3DCOLOR byte order
public:
    unsigned char B, G, R, A;
};

class CCompositeBlendDesc {                          // Ego_r.pdb, 16 bytes
public:
    unsigned long TextureIndex;                       // +0x00
    long          BlendType;                          // +0x04 ECompositeBlendType
    CRGBColour    DiffuseColour;                      // +0x08
    CRGBColour    SpecularColour;                     // +0x0C
};

namespace std {
struct __true_type { __true_type() { } };
struct __false_type { __false_type() { } };

template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}

template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator, _ForwardIterator, const __true_type&) { }

template <class _Tp> struct __type_traits { typedef __false_type has_trivial_destructor; };
template <> struct __type_traits<CCompositeBlendDesc> { typedef __true_type has_trivial_destructor; };

template <class _ForwardIterator, class _Tp>
inline void __destroy(_ForwardIterator __first, _ForwardIterator __last, _Tp*)
{
    typedef typename __type_traits<_Tp>::has_trivial_destructor _Trivial_destructor;
    __destroy_aux(__first, __last, _Trivial_destructor());
}

template <class _Tp> inline void _Destroy(_Tp* __first, _Tp* __last) { __destroy(__first, __last, __first); }

template <class _Tp> class allocator {
public:
    void deallocate(_Tp* __p, unsigned int) { if (__p) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp> class vector {
public:
    typedef _Tp* pointer;
    pointer _M_start;                                                   // +0x00
    pointer _M_finish;                                                  // +0x04
    _STLP_alloc_proxy<pointer, _Tp, allocator<_Tp> > _M_end_of_storage; // +0x08
    ~vector() { _Destroy(_M_start, _M_finish); _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
    void _M_clear();
};
}

class CEngineTextureMorph {                          // Ego_r.pdb; 20 bytes in retail
public:
    long                              OriginalTextureIndex;   // +0x00
    long                              MorphID;                // +0x04
    std::vector<CCompositeBlendDesc>  Blends;                 // +0x08
};

namespace std {
template <class _Tp>
void vector<_Tp>::_M_clear()
{
    _Destroy(_M_start, _M_finish);
    _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
}

template void vector<CEngineTextureMorph>::_M_clear();
}
