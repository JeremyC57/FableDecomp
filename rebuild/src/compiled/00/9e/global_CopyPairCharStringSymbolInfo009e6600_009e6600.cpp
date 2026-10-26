// _global::Copy_PairCharStringSymbolInfo_009e6600(const SPairCharStringSymbolInfo* first, const SPairCharStringSymbolInfo* last, SPairCharStringSymbolInfo* result, const random_access_iterator_tag&, int*) -- assign [first, last) of (name, symbol info) pairs onto result (three string assignments + value), return the advanced result
// Leaf: Copy_PairCharStringSymbolInfo_009e6600
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E6600, length 108 bytes (`ret 0Ch`).
// Identity: `__copy<std::pair<CCharString, CSymbolMap::CSymbolInfo>*>` -- the catalog element is right (16 bytes; the TU holds
//   the landed CVectorMap<CCharString,CSymbolInfo>::LowerBound 0x009E6910), `_Move` is the Dinkumware spelling.
// Callees: CCharString::operator= 0x0099EFB0 (three per element).
// Shape: SGI `__copy(first, last, result, random_access_iterator_tag, Distance*)` (the counted form), `__fastcall`: first in
//   ecx, last in edx, result + the tag reference + the distance pointer on the stack (`ret 0Ch`); the signed count is
//   (last - first) / sizeof(T) (reciprocal multiply), counted down; an empty range returns the untouched result.
//   The nested CSymbolInfo (two strings and a word) gets its own induction pair at +0x0c; the count is kept in the
//   dead `result` slot.
struct random_access_iterator_tag { random_access_iterator_tag() { } };   // empty tag, passed by const reference

class CCharString { public:                            // 4-byte counted string handle
    void* m_pData;
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
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

SPairCharStringSymbolInfo* __fastcall Copy_PairCharStringSymbolInfo_009e6600(const SPairCharStringSymbolInfo* first, const SPairCharStringSymbolInfo* last, SPairCharStringSymbolInfo* result, const random_access_iterator_tag&, int*)
{
    for (int n = last - first; n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}