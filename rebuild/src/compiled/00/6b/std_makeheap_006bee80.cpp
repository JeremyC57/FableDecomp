// std::__make_heap(pair<long, CMusicManager::CRegisteredMusicEntry>* first, pair<..>* last, CKeyPairCompareLess comp, pair<..>*, int*) -- STLport make_heap over the registered-music map: sift every parent down from the last one to the root
// Leaf: ??$__make_heap@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006BEE80, 138 bytes, `ret 0Ch`.
// Callers: the registered-music __partial_sort 0x006BF340 (0x006BF356).
// Callees: CWideString::CWideString(const CWideString&) @ 0x0099B720 (the by-value copy's two strings), __adjust_heap
//   @ 0x006BEBF0 (landed; `__fastcall` first/hole in ecx/edx, len + the 24-byte value + the comparator on the stack).
// Types (Ego_r.pdb): pair<long, CMusicManager::CRegisteredMusicEntry> (24 bytes: key +0, SrcFileName +4, DstFileName +8,
//   Gain +0xC, Offset +0x10, CacheToHD +0x14) and the empty CKeyPairCompareLess<long, CRegisteredMusicEntry>.
// Shape: STLport 4.x `__make_heap(first, last, comp, _Tp*, _Distance*)` from a /Gr TU (`__fastcall`: first in ecx, last
//   in edx, the comparator slot + the two null type pointers on the stack, `ret 0Ch`): `if (last - first < 2) return;
//   len = last - first; parent = (len - 2) / 2; for (;;) { __adjust_heap(first, parent, len, _Tp(*(first + parent)),
//   comp); if (parent == 0) return; parent--; }` -- first kept in the `push ecx` frame word, the element pointer
//   strength-reduced to first + parent * 24 + 0xC, the value built in the argument area by the pair's user copy
//   constructor over the entry's memberwise user copy constructor.
// Label correction: the catalog `Heap_BuildHeapMusic` is descriptive.
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
template <class T1, class T2>
struct pair {
    T1 first;
    T2 second;
    pair(const pair& o) : first(o.first), second(o.second) { }
};
}

typedef std::pair<long, CMusicManager::CRegisteredMusicEntry> CMusicEntryPair;

template <class K, class V>
class CKeyPairCompareLess { public:                            // empty, passed by value
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};

namespace std {
template <class RandomAccessIter, class Distance, class T, class Compare>
void __fastcall __adjust_heap(RandomAccessIter first, Distance holeIndex, Distance len, T value, Compare comp);    // 0x006BEBF0

template <class RandomAccessIter, class Compare, class T, class Distance>
void __fastcall __make_heap(RandomAccessIter first, RandomAccessIter last, Compare comp, T*, Distance*)
{
    if (last - first < 2)
        return;
    Distance len = last - first;
    Distance parent = (len - 2) / 2;
    for (;;) {
        __adjust_heap(first, parent, len, T(*(first + parent)), comp);
        if (parent == 0)
            return;
        parent--;
    }
}

template void __fastcall __make_heap<CMusicEntryPair*, CKeyPairCompareLess<long, CMusicManager::CRegisteredMusicEntry>, CMusicEntryPair, int>(
    CMusicEntryPair*, CMusicEntryPair*, CKeyPairCompareLess<long, CMusicManager::CRegisteredMusicEntry>, CMusicEntryPair*, int*);
}