// std::__copy(first, last, result, const random_access_iterator_tag&, int*) -- (CMap::CScriptedMapBrushUpdateArea*, CMap::CScriptedMapBrushUpdateArea*, int) STLport random-access forward copy: `for (n = last - first; n > 0; --n) { *result = *first; ++first; ++result; }`, returns the advanced result
// Leaf: ??$__copy@PAVCScriptedMapBrushUpdateArea@CMap@@PAV12@H@std@@YIPAVCScriptedMapBrushUpdateArea@CMap@@PAV12@00ABUrandom_access_iterator_tag@0@PAH@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00824790, 61 bytes (+ int3 padding to 64), `ret 0Ch`: /Gr algorithm TU (first ecx, last edx;
//   result, the const random_access_iterator_tag& and the int* distance tag on the stack), spelled here as explicit __fastcall.
// Identity (proven): element CMap::CScriptedMapBrushUpdateArea (8 bytes); evidence: its callers are CMap::UpdateScriptedMapBrushes 0x00820AB0 (which shifts the surviving entries down over an expired one), 0x00824DA0, 0x008254E0 and 0x00825CF0, the insert that the CMap constructors 0x00820CC0 / 0x008224E0 use -- Ego_r.pdb's CMap::ScriptedMapBrushUpdateAreas, vector<CMap::CScriptedMapBrushUpdateArea>, {float Timer; CCharString Name}, CMap's only vector of {4-byte, CCharString} elements
// Label: the manifest's "_Copy_impl<CTCSkeletalMorph::CSkeletalMorph*,CTCSkeletalMorph::CSkeletalMorph*>" is propagated (family H081: six forward copies over a 4-byte field followed by a member with an
//   out-of-line operator=; retail has no identical-code folding, so each address is one element type). Two shapes: 8-byte elements
//   (`sar edx,3`) and 16-byte ones (`sar edx,4`).
// Callee: CCharString::operator= 0x0099EFB0 on Name.
// Shape: the element's implicit operator= inline -- the first dword through eax, then the member's operator= on +4.
class CCharString {                                  // 4 bytes: shared string block
public:
    void* PStringData;
    CCharString& operator=(const CCharString& other);  // 0x0099EFB0
};
class CMap {
public:
    class CScriptedMapBrushUpdateArea {               // 8 bytes (Ego_r.pdb)
    public:
        float       Timer;                            // +0x00
        CCharString Name;                             // +0x04
    };
};

namespace std {

struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class _RandomAccessIter, class _OutputIter, class _Distance>
_OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last,
                              _OutputIter __result, const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}

template CMap::CScriptedMapBrushUpdateArea* __fastcall __copy<CMap::CScriptedMapBrushUpdateArea*, CMap::CScriptedMapBrushUpdateArea*, int>(CMap::CScriptedMapBrushUpdateArea*, CMap::CScriptedMapBrushUpdateArea*, CMap::CScriptedMapBrushUpdateArea*,
                                                                const random_access_iterator_tag&, int*);
}   // namespace std