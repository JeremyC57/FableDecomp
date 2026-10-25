// std::__uninitialized_copy<const CCompositeBlendDesc*, CCompositeBlendDesc*>(first, last, result, const __false_type&)
// -- STLport: copy-construct [first, last) into raw storage at result through the out-of-line copy constructor, return
// the end
// Leaf: ??$__uninitialized_copy@PBVCCompositeBlendDesc@@PAV1@@std@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BAF28, length 43 bytes, `ret 8`:
//   push ebx ; push esi ; mov ebx,edx ; mov esi,ecx ; cmp esi,ebx ; push edi ; mov edi,[esp+10h] ; je E ;
//   L: test edi,edi ; je S ; push esi ; mov ecx,edi ; call CCompositeBlendDesc::CCompositeBlendDesc(const&) 0x005BA48A ;
//   S: add esi,16 ; add edi,16 ; cmp esi,ebx ; jne L ; E: mov eax,edi ; pop edi ; pop esi ; pop ebx ; ret 8
// Identity (FINDINGS chunk 387; was the descriptive batch row UninitCopy_CCompositeBlendDesc_005baf28 with a by-value
//   allocator argument): its callers are the STLport `(const_pointer)__x._M_start` copies of vector<CCompositeBlendDesc>
//   -- the copy constructor 0x005BB346 (0x005BB36D), operator= 0x006CADE0 (0x006CAE37, 0x006CAEC5) and
//   _M_allocate_and_copy 0x006CAD30 (0x006CAD59) -- plus the inline copies of records that own such a vector
//   (__uninitialized_copy<CEngineTextureMorph*> 0x006CB0D0, __uninitialized_fill_n<CEngineTextureMorph*> 0x006CB170,
//   the decal / body-part-mesh helpers 0x0070AA50, 0x0070AAF0, 0x00BEE6DC, ...); the mutable-pointer instantiation is
//   the separate body 0x006CAC20 (no identical-code folding in retail). The callee is the landed
//   CCompositeBlendDesc::CCompositeBlendDesc(const CCompositeBlendDesc&) (Ego_r.pdb ??0CCompositeBlendDesc@@QAE@ABV0@@Z).
// Shape: STLport 4.x `for (; first != last; ++first, ++cur) _Construct(&*cur, *first);` from a /Gr size TU
//   (`__fastcall`: first in ecx, last in edx, result and the empty tag reference on the stack), the placement new
//   null-testing the destination.
#pragma optimize("s", on)

inline void* operator new(unsigned int, void* where) { return where; }

class CCompositeBlendDesc {                             // Ego_r.pdb, 16 bytes
public:
    CCompositeBlendDesc(const CCompositeBlendDesc& other);           // 0x005BA48A
    unsigned char _body[0x10];
};

namespace std {
struct __false_type { __false_type() { } };

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __value) { new ((void*)__p) _T1(__value); }

template <class _InputIter, class _ForwardIter>
_ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}

template CCompositeBlendDesc* __fastcall __uninitialized_copy(const CCompositeBlendDesc*, const CCompositeBlendDesc*,
    CCompositeBlendDesc*, const __false_type&);
}
