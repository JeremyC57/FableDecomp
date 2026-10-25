// _STL::pair::pair(const pair& other) -- copy constructor of pair<CCharString, CSymbolMap::CSymbolInfo>: the key string, then the symbol info's copy constructor
// Leaf: ??0?$pair@VCCharString@@VCSymbolInfo@CSymbolMap@@@_STL@@QAE@ABU01@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009E6050, 55 bytes, __fastcall `ret 4`, returns `this`.
// Identity: the element of CSymbolMap::SLONGMap (Ego_r.pdb: CVectorMap<CCharString, CSymbolMap::CSymbolInfo,
//   CCompareString<CSymbolInfo> >); Ego_r.pdb CSymbolMap::CSymbolInfo (12 bytes): GroupName +0, EntryName +4 (CCharString),
//   Val +8 (long); FableWin.pdb lists its ??0CSymbolInfo@CSymbolMap@@QAE@ABV01@@Z copy constructor. No direct callers
//   (an emitted COMDAT of the vector map's copy helpers).
// Callee: CCharString::CCharString(const CCharString&) @ 0x0099EC30 (three times: first, GroupName, EntryName).
// Shape: `first(p.first), second(p.second)` with the CSymbolInfo copy inlined on its own `this` (ebx = &second, edi = &p.second).
class CCharString {
public:
    void* PData;                                        // +0x00
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
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
template pair<CCharString, CSymbolMap::CSymbolInfo>::pair(const pair<CCharString, CSymbolMap::CSymbolInfo>&);
}