// std::__pop_heap_aux(pair<long,CMusicManager::CRegisteredMusicEntry>* first, pair<...>* last, pair<...>*, CKeyPairCompareLess comp) -- pop the heap top into last-1 (copies the old last element as the sift value)
// Leaf: ??$__pop_heap_aux@PAU?$pair@JVCRegisteredMusicEntry@CMusicManager@@@std@@U12@V?$CKeyPairCompareLess@JVCRegisteredMusicEntry@CMusicManager@@@@@std@@YIXPAU?$pair@JVCRegisteredMusicEntry@CMusicManager@@@0@00V?$CKeyPairCompareLess@JVCRegisteredMusicEntry@CMusicManager@@@@@Z
// Retail: Fable.exe @ 0x006BEF30, 86 bytes, __fastcall (first in ecx, last in edx), `ret 8` (the value-type tag
//   pointer and the empty comparator on the stack).
// Identity (probable): STLport 4.6 `__pop_heap_aux(first, last, T*, comp)` =
//   `__pop_heap(first, last - 1, last - 1, T(*(last - 1)), comp, DISTANCE_TYPE(first))`: it builds the 0x18-byte
//   pair copy in the argument area (two CWideString copy ctors 0x0099B720, then Gain/Offset/CacheToHD) and calls the
//   landed __pop_heap<pair<long, CMusicManager::CRegisteredMusicEntry>*, ..., CKeyPairCompareLess, int> 0x006BEDC0
//   (whose model this reuses) with the null distance pointer; its caller 0x006BEFB0.. is the pop_heap wrapper.
// Callees: CWideString::CWideString(const CWideString&) 0x0099B720, __pop_heap 0x006BEDC0.
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
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};
}

typedef std::pair<long, CMusicManager::CRegisteredMusicEntry> CMusicEntryPair;   // 24 bytes

template <class K, class V> class CKeyPairCompareLess { public:   // empty, passed by value
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};
typedef CKeyPairCompareLess<long, CMusicManager::CRegisteredMusicEntry> TMusicEntryLess;

namespace std {
template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __len, _Tp __val,
                              _Compare __comp);         // 0x006BEBF0

template <class _RandomAccessIterator, class _Tp, class _Compare, class _Distance>
void __fastcall __pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _RandomAccessIterator __result,
                           _Tp __val, _Compare __comp, _Distance*);   // 0x006BEDC0 (landed)

template <class _RandomAccessIterator, class _Tp, class _Compare>
void __fastcall __pop_heap_aux(_RandomAccessIterator __first, _RandomAccessIterator __last, _Tp*, _Compare __comp)
{
    __pop_heap(__first, __last - 1, __last - 1, _Tp(*(__last - 1)), __comp, (int*)0);
}


template void __fastcall __pop_heap_aux(CMusicEntryPair*, CMusicEntryPair*, CMusicEntryPair*, TMusicEntryLess);
}