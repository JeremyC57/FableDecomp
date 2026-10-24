// _global::CopyBackward_PairCharStringSymbolInfo_009e6410(PairCharStringSymbolInfo*, PairCharStringSymbolInfo*, PairCharStringSymbolInfo*, SRandomAccessIteratorTag, long*) -- SGI __copy_backward over pair<CCharString, CSymbolMap::CSymbolInfo>, returns the moved-back result
// Leaf: CopyBackward_PairCharStringSymbolInfo_009e6410
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E6410, length 108 bytes.
// Callees: CCharString::operator= @ 0x0099EFB0 (three per element).
// SGI-STL `__copy_backward(first, last, result, random_access_iterator_tag, Distance*)`, /Gr TU (__fastcall: first in
// ecx, last in edx, result + the two tag arguments on the stack, `ret 0Ch`): `for (n = last - first; n > 0; --n)
// *--result = *--last;` with the signed count computed once; the moved-back result is returned.
// The manifest label is right: Ego_r.pdb CSymbolInfo (12 bytes) = { CCharString GroupName; CCharString EntryName;
// long Val }, so the 16-byte pair is key +0, GroupName +4, EntryName +8, Val +0xC (`sar 4`). The pair's implicit
// operator= calls the out-of-line CCharString::operator= three times and moves Val; the count lives in the dead
// `result` argument slot and the strength-reduced src+0xC/dst+0xC pointers address the nested CSymbolInfo.
struct CCharString {
    void* m_pData;
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};

class CSymbolMap {
public:
    struct CSymbolInfo {                         // Ego_r.pdb: 12 bytes
        CCharString GroupName;                   // +0x00
        CCharString EntryName;                   // +0x04
        long        Val;                         // +0x08
    };
};

struct PairCharStringSymbolInfo {                // std::pair<CCharString, CSymbolMap::CSymbolInfo>, 16 bytes
    CCharString             first;               // +0x00
    CSymbolMap::CSymbolInfo second;              // +0x04
};

struct SRandomAccessIteratorTag { SRandomAccessIteratorTag() { } };

PairCharStringSymbolInfo* __fastcall CopyBackward_PairCharStringSymbolInfo_009e6410(PairCharStringSymbolInfo* first, PairCharStringSymbolInfo* last, PairCharStringSymbolInfo* result, SRandomAccessIteratorTag category, long* distance)
{
    for (long n = last - first; n > 0; --n)
        *--result = *--last;
    return result;
}