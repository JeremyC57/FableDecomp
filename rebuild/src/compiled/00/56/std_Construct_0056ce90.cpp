// std::_Construct(_T1* p, const _T2& value) -- (CUnprovenPod8_0056ce90) STLport placement copy of an 8-byte trivially copyable value: null-checked `new (p) T(value)`, two dword moves
// Leaf: ??$_Construct@UCUnprovenPod8_0056ce90@@U1@@std@@YIXPAUCUnprovenPod8_0056ce90@@ABU1@@Z
// Retail: Fable.exe @ 0x0056CE90, 15 bytes, `__fastcall` (p in ecx, &value in edx), plain `ret`.
// Identity (descriptive element type): the shape is STLport 4.6 `_Construct(_T1* __p, const _T2& __val)`
// (`_construct.h`: `new (__p) _T1(__val)`) instantiated in a /Gr TU (the free template is `__fastcall`), for an 8-byte
// value with an implicit copy (two dword moves through eax/edx). It sits between the CQuestManager boast-map pieces
// (0x0056CB60 pair copy, 0x0056CBA0) and the 0x0056CEA0 release; no direct caller (the xref at 0x00496C9C is not a code
// reference), so the element type cannot be proven: CUnprovenPod8_0056ce90 {two words}.
// Callees: none.
// Shape: `test ecx,ecx ; je` = the new-expression's null test on the placement result; the copy loads +0 / +4 through
// eax / edx in member order.
inline void* operator new(unsigned int, void* where) { return where; }

struct CUnprovenPod8_0056ce90 {                             // 8-byte trivially copyable element (unproven)
    long Word0;                                             // +0x00
    long Word4;                                             // +0x04
};

namespace std {
template <class _T1, class _T2>
inline void __fastcall _Construct(_T1* __p, const _T2& __val)
{
    new (__p) _T1(__val);
}
}

template void __fastcall std::_Construct<CUnprovenPod8_0056ce90, CUnprovenPod8_0056ce90>(CUnprovenPod8_0056ce90*, const CUnprovenPod8_0056ce90&);