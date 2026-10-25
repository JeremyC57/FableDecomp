// std::swap(pair<float,CCharString>& a, pair<float,CCharString>& b) -- exchange two float-keyed string entries through a by-value temporary
// Leaf: ??$swap@
// Retail: Fable.exe (TLC, VC7.1) @ 0x004CFBF0, 78 bytes, /Gr `__fastcall` (a in ecx -> edi, b in edx -> esi), plain `ret`.
// Identity: an 8-byte {4-byte key copied as an integer, CCharString} element in the /Gr STLport block of 0x004CFA70..
//   0x004CFEB0, whose landed __push_heap 0x004CFE10 sorts std::pair<float, CCharString> with CKeyPairCompareLess
//   (FableWin.pdb also sorts pair<float,CCharString> vector maps); no direct callers (an emitted COMDAT whose callers
//   were folded). The catalog's "swap<NParticleEngine::CRegisteredPSCInfo>" is propagated from a byte twin.
// Callees: CCharString copy ctor 0x0099EC30, CCharString::operator= 0x0099EFB0, ~CCharString 0x0099EAE0.
// Shape: STLport `swap(T& a, T& b) { T tmp = a; a = b; b = tmp; }` over STLport's pair (user copy constructor, implicit
//   assignment); a float that is only copied moves through an integer register (`mov eax,[edi]`).
class CCharString {
public:
    void* PData;                                        // +0x00
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
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

template void __fastcall swap<pair<float, CCharString> >(pair<float, CCharString>&, pair<float, CCharString>&);
}