// std::__copy(const CEngineTextureMorph* first, const CEngineTextureMorph* last, CEngineTextureMorph* result, const random_access_iterator_tag&, int*) -- (CEngineTextureMorph) STLport random-access copy: assign last - first elements forward through CEngineTextureMorph's implicit operator=, return the end of the destination
// Leaf: ??$__copy@PBVCEngineTextureMorph@@PAV1@H@std@@YIPAVCEngineTextureMorph@@PBV1@0PAV1@ABUrandom_access_iterator_tag@0@PAH@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00728680, 81 bytes (+ int3 padding), /Gr __fastcall (first in ecx, last in edx), `ret 0Ch`.
// Identity (proven): std::__copy<const CEngineTextureMorph*, CEngineTextureMorph*, int>; evidence: its callers are vector<CEngineTextureMorph>::operator=(const vector&) 0x00728830 (binding ??4?$vector@VCEngineTextureMorph@@V?$allocator@VCEngineTextureMorph@@@std@@@std@@QAEAAV01@ABV01@@Z: __copy_ptrs over the source's const_pointers, CTCLook::OnAppearanceDraw's morph list) and 0x00728780; the 20-byte element copies OriginalTextureIndex +0 and MorphID +4 inline and Blends +8 through vector<CCompositeBlendDesc>::operator= 0x006CADE0; the plain-pointer __copy of CEngineTextureMorph is 0x006CB200 (bound), so this COMDAT is the const-source one.
// Label: the manifest's "_Move<CEngineTextureMorph*,CEngineTextureMorph*>" is propagated from a byte-identical body elsewhere (family J088: the 80/81-byte
//   __copy bodies of {dword, dword, class} elements; retail has no identical-code folding, so each address is its own instantiation).
// Callees: std::vector<CCompositeBlendDesc>::operator=(const vector&) 0x006CADE0 (binding ??4?$vector@VCCompositeBlendDesc@@V?$allocator@VCCompositeBlendDesc@@@std@@@std@@QAEAAV01@ABV01@@Z, FableWin.pdb) (thiscall, `ret 4`), once per element.
// Shape: STLport 4.6 _algobase.h `for (_Distance __n = __last - __first; __n > 0; --__n) { *__result = *__first; ++__first;
//   ++__result; } return __result;` -- the count by the reciprocal divide, the element's implicit operator= inline.
namespace std {
template <class _Tp> class allocator { };
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                         // +0x00
    _Tp* _M_finish;                                        // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;  // +0x08
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    vector<_Tp, _Alloc>& operator=(const vector<_Tp, _Alloc>& __x);   // 0x006CADE0 for CCompositeBlendDesc (out of line)
};
}
class CCompositeBlendDesc;                                  // Ego_r.pdb, 16 bytes
class CEngineTextureMorph {                                     // 20 bytes in retail (24 in Ego_r.pdb); implicit operator=
public:
    long OriginalTextureIndex;                             // +0x00
    long MorphID;                                          // +0x04
    std::vector<CCompositeBlendDesc> Blends;               // +0x08
};
namespace std {
struct input_iterator_tag { };
struct output_iterator_tag { };
struct forward_iterator_tag : public input_iterator_tag { };
struct bidirectional_iterator_tag : public forward_iterator_tag { };
struct random_access_iterator_tag : public bidirectional_iterator_tag { };
template <class _RandomAccessIter, class _OutputIter, class _Distance>
_OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                              const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}
template CEngineTextureMorph* __fastcall __copy<const CEngineTextureMorph*, CEngineTextureMorph*, int>(const CEngineTextureMorph*, const CEngineTextureMorph*, CEngineTextureMorph*,
                                                                const random_access_iterator_tag&, int*);
} // namespace std