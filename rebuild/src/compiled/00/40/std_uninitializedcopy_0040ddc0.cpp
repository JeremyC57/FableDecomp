// std::__uninitialized_copy(CDisplayModeDesc* first, CDisplayModeDesc* last, CDisplayModeDesc* result, const __false_type&) -- STLport element-wise placement copy of display-mode descriptors into raw storage; returns the end of the copied range
// Leaf: ??$__uninitialized_copy@PAVCDisplayModeDesc@@PAV1@@std@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0040DDC0, 56 bytes, `__fastcall` (first ecx, last edx; result and the
//   tag reference on the stack, `ret 8`).
// Identity: every caller copies CDisplayModeDesc (Ego_r: C2DExtentsI Dimensions +0, long BitsPerPixel +8, long
//   RefreshRate +0xc = 16 bytes): the CUserProfileManager ctor 0x0040CEC0, vector<CDisplayModeDesc>::_M_insert_overflow
//   0x004124C0, and the reserve 0x009C2900 / _M_allocate_and_copy 0x009C2030 of the display-mode list built by
//   0x009C02D0 (called only from CUserProfileManager::ApplyVideoValues and its ctor).  Retail was linked without ICF
//   (109 byte-identical tree-decrement bodies and 263 separate `ret 4` stubs stay distinct), so one address is one
//   instantiation; the donor label `_Copy_impl<C2DLineF*,..>` is propagated.  The Ego_r/FableWin name sets hold
//   vector<CDisplayModeDesc> (reserve, _Ufill, _Insert_n ...).
// Shape: STLport 4.6 `__uninitialized_copy(first, last, result, const __false_type&)` (no _STLP_TRY without /EHsc);
//   `_Construct` = placement new with its null test; the element copy is the user (memberwise) copy constructor the PDB name sets list, four dword moves through esi.
#include <new>

class C2DExtentsI {
public:
    long X;                                                 // +0x00
    long Y;                                                 // +0x04
};
class CDisplayModeDesc {
public:
    C2DExtentsI Dimensions;                                 // +0x00
    long        BitsPerPixel;                               // +0x08
    long        RefreshRate;                                // +0x0c
    CDisplayModeDesc();
    CDisplayModeDesc(const CDisplayModeDesc& other)         // PDB ??0CDisplayModeDesc@@QAE@ABV0@@Z: memberwise
        : Dimensions(other.Dimensions), BitsPerPixel(other.BitsPerPixel), RefreshRate(other.RefreshRate) { }
};

namespace std {
struct __false_type { __false_type() { } };

template <class _T1, class _T2>
inline void _Construct(_T1* __p, const _T2& __val)
{
    new (__p) _T1(__val);
}

template <class _InputIter, class _ForwardIter>
_ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result,
                                             const __false_type&)
{
    _ForwardIter __cur = __result;
    for ( ; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}

template CDisplayModeDesc* __fastcall __uninitialized_copy<CDisplayModeDesc*, CDisplayModeDesc*>(
    CDisplayModeDesc*, CDisplayModeDesc*, CDisplayModeDesc*, const __false_type&);
}
