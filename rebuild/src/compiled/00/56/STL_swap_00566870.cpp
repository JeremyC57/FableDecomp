// std::swap(pair<float,CWideString>& a, pair<float,CWideString>& b) -- exchange two float-keyed wide-string entries through a by-value temporary
// Leaf: ??$swap@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00566870, 78 bytes, /Gr `__fastcall` (a in ecx -> edi, b in edx -> esi), plain `ret`.
// Identity: an 8-byte {4-byte key copied as an integer, CWideString} element; FableWin.pdb sorts
//   pair<float, CWideString> vector maps with CKeyPairCompareLess (??$swap@MVCWideString@@@std@@ is its swap), the only
//   {float/long, CWideString} pair the PDBs sort. No direct callers (an emitted COMDAT); byte twin of the
//   pair<float, CCharString> swap 0x004CFBF0 with the CWideString members. The catalog's
//   "swap<NParticleEngine::CRegisteredPSCInfo>" is propagated.
// Callees: CWideString copy ctor 0x0099B720, CWideString::operator= 0x0099B7D0, ~CWideString 0x0099B510.
// Shape: STLport `swap(T& a, T& b) { T tmp = a; a = b; b = tmp; }` over STLport's pair (user copy constructor, implicit
//   assignment); the float key moves through an integer register.
class CWideString {
public:
    void* PData;                                        // +0x00
    CWideString(const CWideString& other);              // 0x0099B720
    ~CWideString();                                     // 0x0099B510
    CWideString& operator=(const CWideString& other);   // 0x0099B7D0
};

namespace std {
template <class _T1, class _T2>
struct pair {
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

template void __fastcall swap<pair<float, CWideString> >(pair<float, CWideString>&, pair<float, CWideString>&);
}