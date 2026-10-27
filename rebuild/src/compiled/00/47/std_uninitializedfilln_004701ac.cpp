// std::uninitialized_fill_n(CAppearanceModifierGraphics::CEntry* __first, unsigned int __n, const CAppearanceModifierGraphics::CEntry& __x) -- construct n copies of x in raw slots, return the end
// Leaf: ??$uninitialized_fill_n@PAUCEntry@CAppearanceModifierGraphics@@IU12@@std@@YIPAUCEntry@CAppearanceModifierGraphics@@PAU12@IABU12@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004701AC, 34 bytes, `__fastcall` (first ecx, n edx, x on the stack), `ret 4` (size-optimised module).
// Identity (proven): its only caller is the _M_fill_insert 0x0047005B of the vector that CPersistContext::Transfer(CAppearanceModifierGraphics&) 0x0046FE40 reads through its TransferBinaryIn 0x0046FEE9 (Ego_r.pdb CAppearanceModifierGraphics: CArray<CAppearanceModifierGraphics::CEntry> at +0; 24-byte elements).
// Shape (family M055, representative 0x0046AEAA): STLport `return __uninitialized_fill_n(__first, __n, __x, __false_type());`
//   with the loop `for (; __n > 0; --__n, ++__cur) _Construct(&*__cur, __x);` inline (`__forceinline`); unsigned n gives `jbe`.
#pragma optimize("s", on)
inline void* __cdecl operator new(unsigned int, void* p) { return p; }
class CAppearanceModifierGraphics {
public:
    struct CEntry { unsigned long Word0, Word1, Word2, Word3, Word4, Word5; };   // 24 bytes (the stride); member names not recovered
};
namespace std {
struct __false_type { __false_type() { } };
template <class _T1, class _T2> __forceinline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _ForwardIter, class _Size, class _Tp>
__forceinline _ForwardIter __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}
template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __fastcall uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x)
{
    return __uninitialized_fill_n(__first, __n, __x, __false_type());
}
template CAppearanceModifierGraphics::CEntry* __fastcall uninitialized_fill_n<CAppearanceModifierGraphics::CEntry*, unsigned int, CAppearanceModifierGraphics::CEntry>(CAppearanceModifierGraphics::CEntry*, unsigned int, const CAppearanceModifierGraphics::CEntry&);
}