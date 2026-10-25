// _STL::pair::operator=(const pair& other) -- copy assignment of pair<CCharString, CSymbolMap::CSymbolInfo>: the key string, then the symbol info's two strings and its value
// Leaf: ??4?$pair@VCCharString@@VCSymbolInfo@CSymbolMap@@@_STL@@QAEAAU01@ABU01@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009E6380, 55 bytes, __fastcall `ret 4`, returns `*this`.
// Identity: the element of CSymbolMap's sorted vector map (Ego_r.pdb: CVectorMap<CCharString, CSymbolMap::CSymbolInfo,
//   CCompareString<CSymbolInfo> >); CSymbolMap::CSymbolInfo (12 bytes): GroupName +0, EntryName +4 (CCharString),
//   Val +8 (long). Twin of the landed-for-staging copy constructor 0x009E6050 (same layout, 0x0099EC30 there). No direct
//   callers: an emitted COMDAT of the vector map's move/copy helpers.
// Callee: CCharString::operator=(const CCharString&) @ 0x0099EFB0 (three times: first, GroupName, EntryName).
// Shape: the compiler-generated pair assignment (STLport's pair declares none) with CSymbolInfo's implicit assignment
//   inlined on its own `this` (ebx = &second, edi = &p.second); the long is stored last, after the callee-saved pops
//   start. The address-of below forces VC7.1 to emit the implicit `??4` out of line.
class CCharString {
public:
    void* PData;                                        // +0x00
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};

class CSymbolMap {
public:
    class CSymbolInfo {
    public:
        CCharString GroupName;                          // +0x00
        CCharString EntryName;                          // +0x04
        long        Val;                                // +0x08
    };
};

namespace _STL {
template <class _T1, class _T2> struct pair {
    _T1 first;                                          // +0x00
    _T2 second;                                         // +0x04
    pair(const pair& p) : first(p.first), second(p.second) {}
};
}

typedef _STL::pair<CCharString, CSymbolMap::CSymbolInfo> TSymbolPair;
TSymbolPair& (TSymbolPair::*g_pAssignSymbolPair)(const TSymbolPair&) = &TSymbolPair::operator=;