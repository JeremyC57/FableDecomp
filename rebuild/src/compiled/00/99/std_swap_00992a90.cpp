// std::swap(_Tp& a, _Tp& b) -- STLport swap of two (4-byte key, CSoundBankRecord) pairs: copy-construct a temporary from a, assign b to a and the temporary to b, destroy the temporary
// Leaf: ??$swap@U?$pair@VCUnprovenKey_00992a90@@VCSoundBankRecord@@@std@@@std@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00992A90, 78 bytes, `__fastcall` (/Gr: a in ecx, b in edx), `ret`.
// Identity (descriptive): std::swap<std::pair<CUnprovenKey_00992a90, CSoundBankRecord> >. No call, jump or data reference reaches it (an emitted
//   COMDAT of a sort); the 4-byte key type is unproven (next to 0x009928A0 (same record, a different 4-byte key type)).
// Callees: the record's copy ctor 0x009922F0, operator= 0x00992630 (twice), dtor 0x00992510.
// Shape (family K117, representative 0x004CF400): STLport `_Tp __tmp = __a; __a = __b; __b = __tmp;` over STLport's pair (user
//   copy constructor: first, then second; implicit memberwise assignment), the J013 swap recipe with an out-of-line second.
class CUnprovenKey_00992a90 {                                             // descriptive: a 4-byte key
public:
    unsigned long Value;
};
class CSoundBankRecord {                                 // 0x20 bytes; the name is the landed stand-in already bound to its copy ctor / operator= / dtor
public:
    CSoundBankRecord(const CSoundBankRecord& other);     // 0x009922F0
    CSoundBankRecord& operator=(const CSoundBankRecord& other);   // 0x00992630
    ~CSoundBankRecord();                                 // 0x00992510
    unsigned char _data[0x20];
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
template void __fastcall swap<pair<CUnprovenKey_00992a90, CSoundBankRecord> >(pair<CUnprovenKey_00992a90, CSoundBankRecord>&, pair<CUnprovenKey_00992a90, CSoundBankRecord>&);
}