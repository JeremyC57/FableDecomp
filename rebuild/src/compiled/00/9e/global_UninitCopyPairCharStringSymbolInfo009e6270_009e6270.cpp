// _global::UninitCopy_PairCharStringSymbolInfo_009e6270(const SPairCharStringSymbolInfo* first, const SPairCharStringSymbolInfo* last, SPairCharStringSymbolInfo* result, const __false_type&) -- placement copy-construct [first, last) of (name, symbol info) pairs, return the advanced result
// Leaf: UninitCopy_PairCharStringSymbolInfo_009e6270
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E6270, length 102 bytes (`ret 8`).
// Identity: catalog `_Move<std::pair<CCharString,CSymbolMap::CSymbolInfo>*>` has the right element (16 bytes; the TU holds the
//   landed CVectorMap<CCharString,CSymbolInfo>::LowerBound 0x009E6910) but the wrong algorithm: this is the null-tested
//   placement copy `__uninitialized_copy`. Callers: fill_n 0x009E62E0, reserve 0x009E72C0, insert-range 0x009E73F0, push_back
//   0x009E77E0.
// Callees: CCharString::CCharString(const CCharString&) 0x0099EC30 (three per element).
// Shape: STLport 4.x `__uninitialized_copy(first, last, result, const __false_type&)`, `__fastcall` (first in ecx, last in
//   edx, result + the tag reference on the stack, `ret 8`); the placement new keeps the `test eax,eax` null test on the
//   destination and the advanced result is returned.
//   `T* cur = result` form (result held in ebp from before the empty test, as 0x009F6C00); the nested
//   CSymbolInfo puts the induction pair at +0x0c (its Val word).
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (the null test on the destination)

struct __false_type { __false_type() { } };     // STLport empty tag; passed by const reference (one stack slot)

class CCharString { public:                            // 4-byte counted string handle
    void* m_pData;
    CCharString(const CCharString& other);      // 0x0099EC30
};

namespace CSymbolMap {
class CSymbolInfo { public:                            // Ego_r.pdb, 12 bytes
    CCharString GroupName;                      // +0x00
    CCharString EntryName;                      // +0x04
    long        Val;                            // +0x08
};
}

struct SPairCharStringSymbolInfo {              // std::pair<CCharString, CSymbolMap::CSymbolInfo>, 16 bytes
    CCharString             first;              // +0x00
    CSymbolMap::CSymbolInfo second;             // +0x04
};

SPairCharStringSymbolInfo* __fastcall UninitCopy_PairCharStringSymbolInfo_009e6270(const SPairCharStringSymbolInfo* first, const SPairCharStringSymbolInfo* last, SPairCharStringSymbolInfo* result, const __false_type&)
{
    SPairCharStringSymbolInfo* cur = result;
    for (; first != last; ++first, ++cur)
        new (cur) SPairCharStringSymbolInfo(*first);
    return cur;
}