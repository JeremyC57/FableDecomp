// std::pair<const CCharString, std::pair<long, CDateAndTime> >::pair(const CCharString&, const pair<long, CDateAndTime>&) -- build a morph-name map value: copy the name, then the (bank index, time stamp) pair
// Leaf: ??0?$pair@$$CBVCCharString@@U?$pair@JVCDateAndTime@@@std@@@std@@QAE@ABVCCharString@@ABU?$pair@JVCDateAndTime@@@1@@Z
// Retail: Fable.exe @ 0x0071EFC0, 45 bytes, thiscall `ret 8`: `call 0x0099EC30` (CCharString copy ctor on this), the
// second pair's long to +4, then its 28-byte CDateAndTime by `rep movsd` (7 dwords) to +8; returns this.
// Identity (probable): the STLport 4.6 two-argument pair ctor `pair(const _T1& __a, const _T2& __b) : first(__a), second(__b)`
// for the value type of the neighbouring landed map<CCharString, pair<long, CDateAndTime> > iterators 0x0071F250 /
// 0x0071F290 (Ego_r.pdb CTCHeroMorph::CustomToBankIndex); the 4 + 28 byte second member matches pair<long, CDateAndTime>
// (Ego_r CDateAndTime is 28 bytes). The row is unreferenced (the scan's ".text 0x004DFA50" xref is a false hit).
// Callee: CCharString::CCharString(const CCharString&) 0x0099EC30.
// Shape: the inner pair's explicit memberwise copy ctor copies the long, then CDateAndTime's implicit (trivial) copy as
// one `rep movsd` block.
class CCharString { public:
    void* PStringData;
    CCharString(const CCharString& other);          // 0x0099EC30
};
class CDateAndTime { public:                        // Ego_r.pdb, 28 bytes
    long          Year;
    unsigned long Month;
    unsigned long Day;
    unsigned long Hour;
    unsigned long Minute;
    unsigned long Second;
    unsigned long Millisecond;
};
namespace std {
template <class _T1, class _T2> struct pair {
    typedef _T1 first_type;
    typedef _T2 second_type;
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};
template struct pair<const CCharString, pair<long, CDateAndTime> >;
}