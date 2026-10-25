// std::__median(const pair<unsigned long,CDefString>& a, const pair<unsigned long,CDefString>& b, const pair<unsigned long,CDefString>& c, CKeyPairCompareLess<unsigned long,CDefString> comp) -- STLport median-of-three for the sound map's criteria-entry introsort pivot: returns a reference to whichever of the three entries has the middle key
// Instantiation: std::__median<pair<unsigned long,CDefString>,CKeyPairCompareLess<unsigned long,CDefString> >
// Leaf: ??$__median@U?$pair@KVCDefString@@@std@@V?$CKeyPairCompareLess@KVCDefString@@@@@std@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004713C5, 45 bytes (size-optimised def TU, no padding), __fastcall
//   (a ecx, b edx; c and the 4-byte comparator slot on the stack), `ret 8`.
// Identity: called by the landed __introsort_loop @ 0x00471338 (first, middle, last - 1) for CSoundMap::CriteriaMap's
//   8-byte pair<unsigned long, CDefString>. The 12-byte CSoundPair twin 0x00470F32 has identical bytes (only the key
//   word is read). The catalog name `std::sort_median_of_three_8byte` is descriptive.
// Shape: STLport 4.6 `__median(a, b, c, comp)` with the comparator's unsigned key compare inlined (`cmp ; jae/jb`); the
//   three keys are loaded once (ebx = c, esi = a, edi = b) and the result is the selected argument register.
#pragma optimize("s", on)

class CDefString {                                          // Ego_r.pdb, 4 bytes (a class: mangled V)
public:
    long TablePos;                                          // +0x0 def-string table index
};

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;                                              // +0x0
    _T2 second;                                             // +0x4
};
}

template <class Key, class Value> class CKeyPairCompareLess {   // empty; passed in a 4-byte slot
public:
    bool operator()(const std::pair<Key, Value>& a, const std::pair<Key, Value>& b) const { return a.first < b.first; }
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

typedef pair<unsigned long, CDefString> _CriteriaEntry;
template const _CriteriaEntry& __fastcall __median(const _CriteriaEntry&, const _CriteriaEntry&, const _CriteriaEntry&,
                                                CKeyPairCompareLess<unsigned long, CDefString>);
}