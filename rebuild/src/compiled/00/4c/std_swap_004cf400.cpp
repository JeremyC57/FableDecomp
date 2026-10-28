// std::swap(_Tp& a, _Tp& b) -- STLport swap of two (4-byte key, CCharString) pairs: copy-construct a temporary from a, assign b to a and the temporary to b, destroy the temporary
// Leaf: ??$swap@U?$pair@VCUnprovenKey_004cf400@@VCCharString@@@std@@@std@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x004CF400, 78 bytes, `__fastcall` (/Gr: a in ecx, b in edx), `ret`.
// Identity (descriptive): std::swap<std::pair<CUnprovenKey_004cf400, CCharString> >. No call, jump or data reference reaches it (an emitted
//   COMDAT of a sort); the 4-byte key type is unproven (next to swap<CDebugSphere> 0x004CF450 and pair<float, CCharString> copy helpers; the catalog's swap<NParticleEngine::CRegisteredPSCInfo> does not fit (that class is 12 bytes)).
// Callees: CCharString copy ctor 0x0099EC30, operator= 0x0099EFB0 (twice), dtor 0x0099EAE0.
// Shape (family K117, representative 0x004CF400): STLport `_Tp __tmp = __a; __a = __b; __b = __tmp;` over STLport's pair (user
//   copy constructor: first, then second; implicit memberwise assignment), the J013 swap recipe with an out-of-line second.
class CUnprovenKey_004cf400 {                                             // descriptive: a 4-byte key
public:
    unsigned long Value;
};
class CCharStringData;
class CCharString {                                      // 4 bytes: PStringData
public:
    CCharString(const CCharString& other);               // 0x0099EC30
    const CCharString& operator=(const CCharString& other);    // 0x0099EFB0
    ~CCharString();                                      // 0x0099EAE0
    CCharStringData* PStringData;
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
template void __fastcall swap<pair<CUnprovenKey_004cf400, CCharString> >(pair<CUnprovenKey_004cf400, CCharString>&, pair<CUnprovenKey_004cf400, CCharString>&);
}