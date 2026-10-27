// std::swap(_Tp& a, _Tp& b) -- STLport swap of two (4-byte key, CWideString) pairs: copy-construct a temporary from a, assign b to a and the temporary to b, destroy the temporary
// Leaf: ??$swap@U?$pair@VCUnprovenKey_00565b60@@VCWideString@@@std@@@std@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00565B60, 78 bytes, `__fastcall` (/Gr: a in ecx, b in edx), `ret`.
// Identity (descriptive): std::swap<std::pair<CUnprovenKey_00565b60, CWideString> >. No call, jump or data reference reaches it (an emitted
//   COMDAT of a sort); the 4-byte key type is unproven (in the CTCHeroOnlineScoreboard TU, among pair<float, CWideString> / map helpers).
// Callees: CWideString copy ctor 0x0099B720, operator= 0x0099B7D0 (twice), dtor 0x0099B510.
// Shape (family K117, representative 0x004CF400): STLport `_Tp __tmp = __a; __a = __b; __b = __tmp;` over STLport's pair (user
//   copy constructor: first, then second; implicit memberwise assignment), the J013 swap recipe with an out-of-line second.
class CUnprovenKey_00565b60 {                                             // descriptive: a 4-byte key
public:
    unsigned long Value;
};
class CWideStringData;
class CWideString {                                      // 4 bytes: PStringData
public:
    CWideString(const CWideString& other);               // 0x0099B720
    CWideString& operator=(const CWideString& other);    // 0x0099B7D0
    ~CWideString();                                      // 0x0099B510
    CWideStringData* PStringData;
};
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;                                          // +0x00
    _T2 second;                                         // +0x04
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) {}
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) {}
};
template <class _Tp>
inline void __fastcall swap(_Tp& __a, _Tp& __b)
{
    _Tp __tmp = __a;
    __a = __b;
    __b = __tmp;
}
template void __fastcall swap<pair<CUnprovenKey_00565b60, CWideString> >(pair<CUnprovenKey_00565b60, CWideString>&, pair<CUnprovenKey_00565b60, CWideString>&);
}