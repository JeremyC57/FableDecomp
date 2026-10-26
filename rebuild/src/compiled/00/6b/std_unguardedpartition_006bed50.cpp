// std::__unguarded_partition(pair<long, CMusicManager::CRegisteredMusicEntry>* first, pair<..>* last, pair<..> pivot, CKeyPairCompareLess comp) -- STLport quicksort partition of the registered-music map by ID; returns the split point
// Leaf: ??$__unguarded_partition@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006BED50, 89 bytes, `ret 1Ch`.
// Callers: the CMusicManager registered-music __introsort_loop.
// Callees: iter_swap<pair*, pair*> @ 0x006BE870 (out of line, `__fastcall`), CWideString::~CWideString() @ 0x0099B510
//   (the by-value pivot's two strings).
// Types (Ego_r.pdb): pair<long, CMusicManager::CRegisteredMusicEntry> (24 bytes: key +0, SrcFileName +4, DstFileName +8,
//   Gain +0xC, Offset +0x10, CacheToHD +0x14) and the empty CKeyPairCompareLess<long, CRegisteredMusicEntry> (signed).
// Shape: STLport 4.x `__unguarded_partition(first, last, _Tp pivot, comp)` from a /Gr TU (`__fastcall`: first in ecx,
//   last in edx, the 24-byte pivot and the comparator slot on the stack, `ret 1Ch`): `for (;;) { while (comp(*first,
//   pivot)) ++first; --last; while (comp(pivot, *last)) --last; if (!(first < last)) return first; iter_swap(first,
//   last); ++first; }` -- the pivot key reloaded at the loop head after the swap call; the pivot's strings destroyed on
//   the way out (DstFileName first). Same source as the CFaction twin 0x006BA100.
// Label correction: the catalog `Heap_PartitionScriptInfo` is descriptive.
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
template <class ForwardIter1, class ForwardIter2>
void __fastcall iter_swap(ForwardIter1 a, ForwardIter2 b);     // 0x006BE870

template <class RandomAccessIter, class T, class Compare>
RandomAccessIter __fastcall __unguarded_partition(RandomAccessIter first, RandomAccessIter last, T pivot, Compare comp)
{
    for (;;) {
        while (comp(*first, pivot))
            ++first;
        --last;
        while (comp(pivot, *last))
            --last;
        if (!(first < last))
            return first;
        iter_swap(first, last);
        ++first;
    }
}

template CMusicEntryPair* __fastcall __unguarded_partition<CMusicEntryPair*, CMusicEntryPair, CKeyPairCompareLess<long, CMusicManager::CRegisteredMusicEntry> >(
    CMusicEntryPair*, CMusicEntryPair*, CMusicEntryPair, CKeyPairCompareLess<long, CMusicManager::CRegisteredMusicEntry>);
}