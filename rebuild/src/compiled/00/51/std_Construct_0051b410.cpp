// std::_Construct(CRegion* p, const CRegion& value) -- STLport placement copy-construct of a region (null-tested placement new)
// Flags: /Gr
// Leaf: ??$_Construct@VCRegion@@V1@@std@@YIXPAVCRegion@@ABV1@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0051B410, 11 bytes, __fastcall (p in ecx, &value in edx), plain `ret`.
//   An out-of-line COMDAT with no direct caller (the xref.py hit 0x0047F7DC decodes to no instruction).
// Identity: the callee 0x0051A900 is CRegion::CRegion(const CRegion&) (catalog; bound ??0CRegion@@QAE@ABV0@@Z by the
//   landed 0x0051BEC0); Ego_r.pdb has the same family as ??$_Construct@VCRegion@@ABV1@@std@@ (its library's
//   `const T2&` template argument); this is the STLport 4.6 spelling `_Construct<CRegion, CRegion>`.
// Shape: placement new tests the address (`test ecx,ecx ; je`), then the out-of-line copy ctor with `push edx`.
class CRegion {                                   // Ego_r.pdb class (V)
public:
    unsigned char _body[0x4];
    CRegion(const CRegion& other);                // 0x0051A900
};

inline void* operator new(unsigned int, void* p) { return p; }   // placement new (STLport <new>)

namespace std {
template <class _T1, class _T2> void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template void _Construct<CRegion, CRegion>(CRegion*, const CRegion&);
}