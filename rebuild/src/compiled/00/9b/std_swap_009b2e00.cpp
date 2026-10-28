// std::swap(pair<CCharString, unsigned long>& a, pair<CCharString, unsigned long>& b) -- exchange two pairs through a copy
// Leaf: ??$swap@U?$pair@VCCharString@@K@std@@@std@@YIXAAU?$pair@VCCharString@@K@0@0@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009b2e00, 74 bytes, /Gr `__fastcall` (a in ecx, b in edx), `ret`.
// Identity (probable): no direct caller; it sits among the same TU's insertion-sort / heap helpers, whose CCharString-keyed instantiation is pair<CCharString, unsigned long> (bound __median 0x009B5730, __unguarded_partition 0x009B5800, __adjust_heap 0x009B5C70, __introsort_loop 0x009B8B50 -- the partition's swaps), the 8-byte element its body copies.
// Callees: CCharString::CCharString(const CCharString&) 0x0099EC30, CCharString::operator=(const CCharString&) 0x0099EFB0 (twice),
//   CCharString::~CCharString 0x0099EAE0.
// Body: STLport 4.6 `_Tp __tmp = __a; __a = __b; __b = __tmp;` -- pair's user copy constructor builds the temporary, the implicit
//   memberwise assignment copies each half.
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
template void __fastcall swap<pair<CCharString, unsigned long > >(pair<CCharString, unsigned long >&, pair<CCharString, unsigned long >&);
}