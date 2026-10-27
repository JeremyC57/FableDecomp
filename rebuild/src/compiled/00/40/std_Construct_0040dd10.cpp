// std::_Construct(T1* p, const T2& value) -- placement-copy-construct one 16-byte trivially copyable record (STLport, null-checked placement new)
// Leaf: ??$_Construct@VCUnprovenRecord16_0040dd10@@V1@@std@@YIXPAVCUnprovenRecord16_0040dd10@@ABV1@@Z
// Retail: Fable.exe @ 0x0040DD10, 27 bytes, register convention modelled as `__fastcall` (p in ecx, value in edx), as
// the landed sibling std::_Construct<CWideString,CWideString> 0x0040DD00 directly before it: `test ecx,ecx ; je` (the
// throw() placement new's null test), then four dword moves.
// Identity: the STLport `_Construct(_T1*, const _T2&)` instance of this TU for a 16-byte POD record; it has no direct
// caller in retail (gap-scan COMDAT), so the record type is unproven: CUnprovenRecord16_0040dd10 (descriptive).
inline void* operator new(unsigned int, void* where) throw() { return where; }

class CUnprovenRecord16_0040dd10 {
public:
    unsigned long Word0;                                // +0x00
    unsigned long Word1;                                // +0x04
    unsigned long Word2;                                // +0x08
    unsigned long Word3;                                // +0x0C
};

namespace std {
template <class T1, class T2> void __fastcall _Construct(T1* p, const T2& value)
{
    new (p) T1(value);
}
template void __fastcall _Construct<CUnprovenRecord16_0040dd10, CUnprovenRecord16_0040dd10>(
    CUnprovenRecord16_0040dd10* p, const CUnprovenRecord16_0040dd10& value);
}