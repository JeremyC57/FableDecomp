// std::__median(const pair<CCharString, CSymbolMap::CSymbolInfo>& a, const pair<...>& b, const pair<...>& c, CCompareString<CSymbolMap::CSymbolInfo> comp) -- the median of three pairs by their CCharString key
// Leaf: ??$__median@U?$pair@VCCharString@@VCSymbolInfo@CSymbolMap@@@std@@V?$CCompareString@VCSymbolInfo@CSymbolMap@@@@@std@@YIABU?$pair@VCCharString@@VCSymbolInfo@CSymbolMap@@@0@ABU10@00V?$CCompareString@VCSymbolInfo@CSymbolMap@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009e6c20, 94 bytes, /Gr `__fastcall` (a in ecx, b in edx, c and the comparator on the stack), `ret 8`.
// Identity (probable): no direct caller (the TU's introsort inlines it; out-of-line copy kept); the symbol-map TU's only CCharString-keyed sort is over CSymbolMap::SLONGMap (Ego_r.pdb CVectorMap<CCharString, CSymbolMap::CSymbolInfo, CCompareString<CSymbolMap::CSymbolInfo> >; bound neighbours __push_heap 0x009E67C0, __adjust_heap 0x009E6DB0, __pop_heap 0x009E7060, __make_heap 0x009E7100 of that pair and comparator).
// Callee: CCharString::operator<(const CCharString&) const 0x009E6530 (Ego_r.pdb ??MCCharString@@QBE_NABV0@@Z; the comparator inline).
// Body: STLport 4.6 __median (the nested comp(a, b) / comp(b, c) / comp(a, c) ladder returning a reference).
class CCharString {
public:
    void* PStringData;
    bool operator<(const CCharString& other) const;        // 0x009E6530
};
class CSymbolMap {
public:
    class CSymbolInfo {                                    // Ego_r.pdb, 12 bytes
    public:
        CCharString GroupName;                             // +0x00
        CCharString EntryName;                             // +0x04
        long        Val;                                   // +0x08
    };
};
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;                                             // +0x00
    _T2 second;                                            // +0x04
};
}
template <class T> class CCompareString {
public:
    bool operator()(const std::pair<CCharString, T>& a, const std::pair<CCharString, T>& b) const { return a.first < b.first; }
};
namespace std {
template <class _Tp, class _Compare>
const _Tp& __fastcall __median(const _Tp& __a, const _Tp& __b, const _Tp& __c, _Compare __comp)
{
    if (__comp(__a, __b))
        if (__comp(__b, __c))
            return __b;
        else if (__comp(__a, __c))
            return __c;
        else
            return __a;
    else if (__comp(__a, __c))
        return __a;
    else if (__comp(__b, __c))
        return __c;
    else
        return __b;
}
template const pair<CCharString, CSymbolMap::CSymbolInfo >& __fastcall __median(const pair<CCharString, CSymbolMap::CSymbolInfo >&, const pair<CCharString, CSymbolMap::CSymbolInfo >&, const pair<CCharString, CSymbolMap::CSymbolInfo >&, CCompareString<CSymbolMap::CSymbolInfo >);
}