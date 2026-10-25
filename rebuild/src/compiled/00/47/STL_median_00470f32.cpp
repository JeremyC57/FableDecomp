// _STL::__median(const pair<unsigned long,CSoundPair>& a, const pair<unsigned long,CSoundPair>& b, const pair<unsigned long,CSoundPair>& c, CKeyPairCompareLess<unsigned long,CSoundPair> comp) -- STLport median-of-three for the sound map's introsort pivot: returns a reference to whichever of the three entries has the middle key
// Instantiation: _STL::__median<pair<unsigned long,CSoundPair>,CKeyPairCompareLess<unsigned long,CSoundPair> >
// Leaf: ??$__median@U?$pair@KVCSoundPair@@@_STL@@V?$CKeyPairCompareLess@KVCSoundPair@@@@@_STL@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00470F32, 45 bytes (size-optimised def TU, no padding), __fastcall
//   (a ecx, b edx; c and the 4-byte comparator slot on the stack), `ret 8`.
// Identity: called by the landed __introsort_loop @ 0x00470E9E (first, middle, last - 1). The 8-byte CDefString twin
//   0x004713C5 has identical bytes (only the key word is read). The catalog name `std::sort_median_of_three_12byte` is
//   descriptive.
// Shape: STLport 4.6 `__median(a, b, c, comp)` with the comparator's unsigned key compare inlined (`cmp ; jae/jb`); the
//   three keys are loaded once (ebx = c, esi = a, edi = b) and the result is the selected argument register.
#pragma optimize("s", on)

class CSoundPair {                                          // Ego_r.pdb, 8 bytes (a class: mangled V)
public:
    long FirstSound;                                        // +0x0
    long LastSound;                                         // +0x4
};

namespace _STL {
template <class _T1, class _T2> struct pair {
    _T1 first;                                              // +0x0
    _T2 second;                                             // +0x4
};
}

template <class Key, class Value> class CKeyPairCompareLess {   // empty; passed in a 4-byte slot
public:
    bool operator()(const _STL::pair<Key, Value>& a, const _STL::pair<Key, Value>& b) const { return a.first < b.first; }
};

namespace _STL {
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

typedef pair<unsigned long, CSoundPair> _SoundEntry;
template const _SoundEntry& __fastcall __median(const _SoundEntry&, const _SoundEntry&, const _SoundEntry&,
                                                CKeyPairCompareLess<unsigned long, CSoundPair>);
}