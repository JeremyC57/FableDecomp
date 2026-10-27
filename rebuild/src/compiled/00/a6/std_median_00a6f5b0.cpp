// std::__median(const pair<CCharString, C3DHelperPoint>& a, const pair<...>& b, const pair<...>& c, CCompareString<C3DHelperPoint> comp) -- the median of three pairs by their CCharString key
// Leaf: ??$__median@U?$pair@VCCharString@@VC3DHelperPoint@@@std@@V?$CCompareString@VC3DHelperPoint@@@@@std@@YIABU?$pair@VCCharString@@VC3DHelperPoint@@@0@ABU10@00V?$CCompareString@VC3DHelperPoint@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00a6f5b0, 94 bytes, /Gr `__fastcall` (a in ecx, b in edx, c and the comparator on the stack), `ret 8`.
// Identity (probable): no direct caller; it precedes the __unguarded_partition 0x00A6F610 called by the introsort loop (0x00A73079) whose depth fallback is the __partial_sort 0x00A72300 that runs sort_heap<pair<CCharString, C3DHelperPoint>*, CCompareString<C3DHelperPoint> > 0x00A719A0 (bound).
// Callee: CCharString::operator<(const CCharString&) const 0x009E6530 (Ego_r.pdb ??MCCharString@@QBE_NABV0@@Z; the comparator inline).
// Body: STLport 4.6 __median (the nested comp(a, b) / comp(b, c) / comp(a, c) ladder returning a reference).
class CCharString {
public:
    void* PStringData;
    bool operator<(const CCharString& other) const;        // 0x009E6530
};
class C3DHelperPoint { public: unsigned char Data[0x10]; };   // Ego_r.pdb 16 bytes (members not touched here)
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
template const pair<CCharString, C3DHelperPoint >& __fastcall __median(const pair<CCharString, C3DHelperPoint >&, const pair<CCharString, C3DHelperPoint >&, const pair<CCharString, C3DHelperPoint >&, CCompareString<C3DHelperPoint >);
}