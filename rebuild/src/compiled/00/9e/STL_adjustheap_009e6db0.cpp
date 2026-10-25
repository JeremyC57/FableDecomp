// std::__adjust_heap(pair<CCharString,CSymbolMap::CSymbolInfo>* first, int holeIndex, int len, pair<CCharString,CSymbolMap::CSymbolInfo> val, CCompareString comp) -- STLport heap sift-down of the symbol map's (name, symbol info) table by CCharString::FastCompare, then __push_heap val back up
// Leaf: ??$__adjust_heap@PAU?$pair@VCCharString@@VCSymbolInfo@CSymbolMap@@@std@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x009E6DB0, 287 bytes (+ int3 padding), `ret 18h` (__fastcall: first in ecx,
//   holeIndex in edx; len, the 16-byte value and the empty comparator on the stack: 4 + 0x10 + 4 = 0x18).
// Element (16 bytes, `shl eax,4`): CCharString key +0, CSymbolMap::CSymbolInfo +4 (Ego_r.pdb, 0xC bytes:
//   CCharString GroupName +0, CCharString EntryName +4, long Val +8).
// Callees: CCharString::FastCompare(const CCharString&) @ 0x009E6530 (out of line, thiscall on first[second].first:
//   length first, then the bytes -- the CCompareString predicate), CCharString::operator= @ 0x0099EFB0 x3 (implicit
//   element assignment, then the Val dword), CCharString(const CCharString&) @ 0x0099EC30 x3 (memberwise pair copy
//   built in __push_heap's argument slot), __push_heap @ 0x009E67C0 (same instantiation), ~CCharString @ 0x0099EAE0 x3.
// Callers: __pop_heap 0x009E7060 and the make_heap / sort family of the same table.
// Shape: STLport 4.6 `__adjust_heap`; topIndex homed in the `push ecx` slot; `comp(first[second], first[second-1])`.
class CCharString {
public:
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
    bool FastCompare(const CCharString& other) const;   // 0x009E6530
private:
    void* PStringData;
};

class CSymbolMap {
public:
    class CSymbolInfo {                         // Ego_r.pdb, 0xC bytes
    public:
        CCharString GroupName;                  // +0x0
        CCharString EntryName;                  // +0x4
        long        Val;                        // +0x8
    };
};

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) {}
};
}

template <class T> class CCompareString {       // empty (Ego_r.pdb: 1 byte)
public:
    bool operator()(const std::pair<CCharString, T>& a, const std::pair<CCharString, T>& b) const { return a.first.FastCompare(b.first); }
};
typedef std::pair<CCharString, CSymbolMap::CSymbolInfo> CSymbolPair;

namespace std {
template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __push_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __topIndex, _Tp __val, _Compare __comp);   // 0x009E67C0

template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __len, _Tp __val, _Compare __comp)
{
    _Distance __topIndex = __holeIndex;
    _Distance __secondChild = 2 * __holeIndex + 2;
    while (__secondChild < __len) {
        if (__comp(*(__first + __secondChild), *(__first + (__secondChild - 1))))
            __secondChild--;
        *(__first + __holeIndex) = *(__first + __secondChild);
        __holeIndex = __secondChild;
        __secondChild = 2 * (__secondChild + 1);
    }
    if (__secondChild == __len) {
        *(__first + __holeIndex) = *(__first + (__secondChild - 1));
        __holeIndex = __secondChild - 1;
    }
    __push_heap(__first, __holeIndex, __topIndex, __val, __comp);
}

template void __fastcall __adjust_heap(CSymbolPair*, int, int, CSymbolPair, CCompareString<CSymbolMap::CSymbolInfo>);
}