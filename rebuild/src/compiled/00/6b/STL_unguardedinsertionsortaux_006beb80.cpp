// std::__unguarded_insertion_sort_aux(pair<long,CMusicManager::CRegisteredMusicEntry>* first, pair<...>* last, pair<...>*, CKeyPairCompareLess<long,CMusicManager::CRegisteredMusicEntry> comp) -- STLport final unguarded insertion pass over the registered-music map
// Leaf: ??$__unguarded_insertion_sort_aux@
// Retail: Fable.exe (TLC, VC7.1) @ 0x006BEB80, 101 bytes (+ int3 padding), /Gr `__fastcall` (first in ecx, last in edx
//   -> ebp; the value-type tag pointer and the comparator word on the stack, `ret 8`).
// Identity: STLport 4.6 `__unguarded_insertion_sort_aux` of the CMusicManager registered-music CVectorMap sort; the
//   callee is the landed __unguarded_linear_insert 0x006BE930 and the TU holds the landed __pop_heap 0x006BEDC0 /
//   __adjust_heap 0x006BEBF0 of the same 24-byte pair<long, CMusicManager::CRegisteredMusicEntry> (Ego_r.pdb entry:
//   CWideString SrcFileName +0, DstFileName +4, float Gain +8, Offset +0xc, bool CacheToHD +0x10). Callers: 0x006BEDB7,
//   0x006BF4B6. The catalog's "vector<CGraphicInfo>::_Destroy" is propagated.
// Callees: CWideString copy ctor 0x0099B720 (twice: the `_Tp(*__i)` copy built in the argument area),
//   __unguarded_linear_insert 0x006BE930 (out of line; it owns and destroys its by-value val).
// Shape: `for (__i = __first; __i != __last; ++__i) __unguarded_linear_insert(__i, _Tp(*__i), __comp);` -- the pair's
//   user copy constructor over the entry's memberwise user copy constructor (the landed __pop_heap model) builds the
//   24-byte argument in its stack slot; the comparator stays declaration-only.
class CWideString {
public:
    void* PStringData;                                  // +0x00
    CWideString(const CWideString& other);              // 0x0099B720
    CWideString& operator=(const CWideString& other);   // 0x0099B7D0
    ~CWideString();                                     // 0x0099B510
};

class CMusicManager {
public:
    class CRegisteredMusicEntry {                       // 20 bytes
    public:
        CWideString SrcFileName;                        // +0x00
        CWideString DstFileName;                        // +0x04
        float       Gain;                               // +0x08
        float       Offset;                             // +0x0C
        bool        CacheToHD;                          // +0x10
        CRegisteredMusicEntry(const CRegisteredMusicEntry& o)
            : SrcFileName(o.SrcFileName), DstFileName(o.DstFileName), Gain(o.Gain), Offset(o.Offset), CacheToHD(o.CacheToHD) {}
    };
};

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;                                          // +0x00
    _T2 second;                                         // +0x04
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) {}
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) {}
};

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp);   // 0x006BE930

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_insertion_sort_aux(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Compare __comp)
{
    for (_RandomAccessIter __i = __first; __i != __last; ++__i)
        __unguarded_linear_insert(__i, _Tp(*__i), __comp);
}
}

template <class K, class V> struct CKeyPairCompareLess {   // empty, passed by value (one stack word)
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const;
};

typedef std::pair<long, CMusicManager::CRegisteredMusicEntry> TMusicEntryPair;
typedef CKeyPairCompareLess<long, CMusicManager::CRegisteredMusicEntry> TMusicEntryLess;
template void __fastcall std::__unguarded_insertion_sort_aux<TMusicEntryPair*, TMusicEntryPair, TMusicEntryLess>(
    TMusicEntryPair*, TMusicEntryPair*, TMusicEntryPair*, TMusicEntryLess);