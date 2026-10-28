// std::swap(pair<CCharString, CUnprovenPointee_00a3fd60*>& a, pair<CCharString, CUnprovenPointee_00a3fd60*>& b) -- exchange two pairs through a copy
// Leaf: ??$swap@U?$pair@VCCharString@@PAVCUnprovenPointee_00a3fd60@@@std@@@std@@YIXAAU?$pair@VCCharString@@PAVCUnprovenPointee_00a3fd60@@@0@0@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00a3fd60, 74 bytes, /Gr `__fastcall` (a in ecx, b in edx), `ret`.
// Identity (descriptive): no direct caller; a second byte-identical {CCharString, dword} swap in the sound-bank code after __copy<pair<CCharString, CSoundBankLH*>*> 0x00A3FCD0 -- it cannot be the same instantiation as 0x00A3F750 (one symbol per address), so its element type is unproven.
// Callees: CCharString::CCharString(const CCharString&) 0x0099EC30, CCharString::operator=(const CCharString&) 0x0099EFB0 (twice),
//   CCharString::~CCharString 0x0099EAE0.
// Body: STLport 4.6 `_Tp __tmp = __a; __a = __b; __b = __tmp;` -- pair's user copy constructor builds the temporary, the implicit
//   memberwise assignment copies each half.
class CUnprovenPointee_00a3fd60;                       // descriptive: the second member is unproven
class CCharString {
public:
    void* PStringData;
    CCharString(const CCharString& other);                 // 0x0099EC30
    const CCharString& operator=(const CCharString& other);      // 0x0099EFB0
    ~CCharString();                                        // 0x0099EAE0
};
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;                                             // +0x00
    _T2 second;                                            // +0x04
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }   // STLport 4.6's user copy constructor
};
template <class _Tp> void __fastcall swap(_Tp& __a, _Tp& __b)
{
    _Tp __tmp = __a;
    __a = __b;
    __b = __tmp;
}
template void __fastcall swap<pair<CCharString, CUnprovenPointee_00a3fd60* > >(pair<CCharString, CUnprovenPointee_00a3fd60* >&, pair<CCharString, CUnprovenPointee_00a3fd60* >&);
}