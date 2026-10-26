// std::__unguarded_linear_insert(pair<long, CMusicManager::CRegisteredMusicEntry>* last, pair<..> val, CKeyPairCompareLess comp) -- STLport insertion-sort step over the registered-music map: shift larger IDs up one slot, drop val into the hole
// Leaf: ??$__unguarded_linear_insert@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006BE930, 159 bytes (+ int3 padding), `ret 1Ch`.
// Callers: the CMusicManager registered-music insertion sort helpers (__linear_insert 0x006BF020, the unguarded
//   insertion sort).
// Callees: CWideString::operator=(const CWideString&) @ 0x0099B7D0, CWideString::~CWideString() @ 0x0099B510.
// Types (Ego_r.pdb): pair<long, CMusicManager::CRegisteredMusicEntry> (24 bytes: key +0, SrcFileName +4, DstFileName +8,
//   Gain +0xC, Offset +0x10, CacheToHD +0x14) and the empty CKeyPairCompareLess<long, CRegisteredMusicEntry> (signed key
//   compare, `jge` / `jl`).
// Shape: STLport 4.x `__unguarded_linear_insert(last, _Tp val, comp)` from a /Gr TU (`__fastcall`: last in ecx, the
//   24-byte val and the comparator slot on the stack, `ret 1Ch`): `next = last; --next; while (comp(val, *next)) {
//   *last = *next; last = next; --next; } *last = val;` -- the pair's implicit memberwise operator= (key, two string
//   assignments, three POD words; a second induction pointer at next + 0xC), then val's two string destructors.
// Label correction: the catalog `Vector_ShiftElementsWideString` is descriptive.
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
template <class RandomAccessIter, class T, class Compare>
void __fastcall __unguarded_linear_insert(RandomAccessIter last, T val, Compare comp)
{
    RandomAccessIter next = last;
    --next;
    while (comp(val, *next)) {
        *last = *next;
        last = next;
        --next;
    }
    *last = val;
}

template void __fastcall __unguarded_linear_insert<CMusicEntryPair*, CMusicEntryPair, CKeyPairCompareLess<long, CMusicManager::CRegisteredMusicEntry> >(
    CMusicEntryPair*, CMusicEntryPair, CKeyPairCompareLess<long, CMusicManager::CRegisteredMusicEntry>);
}