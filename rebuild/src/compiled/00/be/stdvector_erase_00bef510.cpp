// std::vector::erase(iterator first, iterator last) -- (vector<std::pair<std::pair<long, long>, CEngineTextureDecal> >) STLport range erase: assign the tail [last, finish) down onto first through the out-of-line __copy, free each vacated element's owned block (member at +0x10), set finish to the copy's end, return first
// Leaf: ?erase@?$vector@U?$pair@U?$pair@JJ@std@@VCEngineTextureDecal@@@std@@V?$allocator@U?$pair@U?$pair@JJ@std@@VCEngineTextureDecal@@@std@@@2@@std@@QAEPAU?$pair@U?$pair@JJ@std@@VCEngineTextureDecal@@@2@PAU32@0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BEF510, 99 bytes (+ int3 padding to 112), thiscall, `ret 8`, returns first.
// Identity (proven): std::vector<std::pair<std::pair<long, long>, CEngineTextureDecal> >::erase(iterator, iterator); evidence: 0x00BECDE0 -- CEngineInternalPrimitiveMorphInfo::InitialiseTextureDecals(const vector<CEngineTextureDecal>&) (Ego_r.pdb; it takes a 20-byte-element array, `imul 0x66666667; sar 3`) -- erases [begin, end) of this+0 and rebuilds it from the decals; Ego_r.pdb CEngineInternalPrimitiveMorphInfo::TextureDecals at +0 is CVectorMap<pair<long, long>, CEngineTextureDecal>, whose 28-byte pair holds the decal's OverlaidBlends block at +0x10; the caller is CEngineInternalPrimitiveMorphedAnimatedMesh's 0x00BD2B70 on its Morph at +0x128; FableWin.pdb holds ?erase@?$vector@U?$pair@U?$pair@JJ@std@@VCEngineTextureDecal@@.
// Label: the manifest's "CRandomAppearanceMorph::CleanupMorphs" is propagated (family J010: six erase bodies whose element owns one malloc block in a
//   member past +0 -- `lea esi, [ebx + 10h]` / `add esi, 1Ch`; retail has no identical-code folding, so each
//   address is one element type).
// Callees: std::__copy<std::pair<std::pair<long, long>, CEngineTextureDecal>*, std::pair<std::pair<long, long>, CEngineTextureDecal>*, int> 0x00BEE7B0 (`__fastcall`: last in ecx, finish in edx; first, the
//   `random_access_iterator_tag` reference and `(int*)0` on the stack; retail homes the empty tag in the LIVE `first`
//   slot, which VC7.1 does only when it sees the same-TU __copy body never touches the tag, so the body is given here),
//   free 0x00BFEA14 (each vacated element's inlined member `_Vector_base` destructor).
// Body: STLport 4.6 `pointer __i = __copy_ptrs(__last, _M_finish, __first, _TrivialAss()); _Destroy(__i, _M_finish);
//   _M_finish = __i; return __first;` with __copy_ptrs(__false_type) = `__copy(first, last, result,
//   random_access_iterator_tag(), (ptrdiff_t*)0)` and _Destroy = the __destroy_aux(__false_type) loop of `p->~T()`.
// Model: family G041 (agent951; same body with the block at +0).
#include <stddef.h>
extern "C" void __cdecl free(void* block);                      // 0x00BFEA14
namespace std {
struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };
template <class _Tp> class allocator { public: allocator() { } };
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc>
class _Vector_base {
public:
    ~_Vector_base() { if (_M_start) free(_M_start); }
    _Tp* _M_start;                                              // +0x00
    _Tp* _M_finish;                                             // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;     // +0x08
};
template <class _RandomAccessIter, class _OutputIter, class _Distance>                  // 0x00BEE7B0 (same-TU body)
__declspec(noinline) _OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last,
                                                   _OutputIter __result, const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}
template <class _Tp>
inline _Tp* __copy_ptrs(_Tp* __first, _Tp* __last, _Tp* __result, const __false_type&)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (ptrdiff_t*)0);
}
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for ( ; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _Tp> inline void _Destroy(_Tp* __first, _Tp* __last) { __destroy_aux(__first, __last, __false_type()); }
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* pointer;
    typedef _Tp* iterator;
    typedef __false_type _TrivialAss;
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
    vector<_Tp, _Alloc>& operator=(const vector<_Tp, _Alloc>& __x);
    iterator erase(iterator __first, iterator __last)
    {
        pointer __i = __copy_ptrs(__last, this->_M_finish, __first, _TrivialAss());
        _Destroy(__i, this->_M_finish);
        this->_M_finish = __i;
        return __first;
    }
};
template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
}
enum ECompositeBlendType { COMPOSITE_BLEND_TYPE_0 };
class CRGBColour { public: unsigned char B, G, R, A; };          // 4 bytes
class CCompositeBlendDesc {                                       // 16 bytes (Ego_r.pdb)
public:
    unsigned long       TextureIndex;
    ECompositeBlendType BlendType;
    CRGBColour          DiffuseColour;
    CRGBColour          SpecularColour;
};
class CEngineTextureDecal {                                       // 20 bytes in retail (Ego_r.pdb members)
public:
    long OriginalTextureIndex;                                    // +0x00
    long DecalID;                                                 // +0x04
    std::vector<CCompositeBlendDesc> OverlaidBlends;              // +0x08
};
template std::vector<std::pair<std::pair<long, long>, CEngineTextureDecal> >::iterator std::vector<std::pair<std::pair<long, long>, CEngineTextureDecal> >::erase(std::vector<std::pair<std::pair<long, long>, CEngineTextureDecal> >::iterator, std::vector<std::pair<std::pair<long, long>, CEngineTextureDecal> >::iterator);