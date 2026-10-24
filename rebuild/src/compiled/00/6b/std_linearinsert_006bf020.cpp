// std::__linear_insert(pair<long, CMusicManager::CRegisteredMusicEntry>* first, pair<..>* last, pair<..> val, CKeyPairCompareLess comp) -- STLport insertion-sort step over the registered-music map: a new minimum shifts the prefix up, anything else takes the unguarded walk
// Leaf: ??$__linear_insert@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006BF020, 180 bytes (+ int3 padding), `ret 1Ch`.
// Callers: the registered-music __insertion_sort 0x006BF2C0 (landed).
// Callees: __copy_backward<pair*, pair*, int> @ 0x006BEAA0 (landed), CWideString::operator= @ 0x0099B7D0,
//   CWideString::CWideString(const CWideString&) @ 0x0099B720, __unguarded_linear_insert @ 0x006BE930,
//   CWideString::~CWideString() @ 0x0099B510.
// Types (Ego_r.pdb): pair<long, CMusicManager::CRegisteredMusicEntry> (24 bytes: key +0, SrcFileName +4, DstFileName +8,
//   Gain +0xC, Offset +0x10, CacheToHD +0x14) and the empty CKeyPairCompareLess<long, CRegisteredMusicEntry> (signed).
// Shape: STLport 4.x `__linear_insert(first, last, _Tp val, comp)` from a /Gr TU (`__fastcall`: first in ecx, last in
//   edx, the 24-byte val and the comparator slot on the stack, `ret 1Ch`): `if (comp(val, *first)) { copy_backward(first,
//   last, last + 1); *first = val; } else __unguarded_linear_insert(last, val, comp);` -- copy_backward is the inline
//   `__copy_backward_ptrs` forwarder (its `random_access_iterator_tag()` temporary homed in the dead comparator slot, a
//   null distance pointer), the pair's implicit operator= inline (key, two string assignments, three POD words), the
//   second val copy built in the argument area, then val's two string destructors on the shared exit. Same source as
//   the CFaction twin 0x006BA7B0 (where the pair assignment stays out of line).
// Label correction: the catalog `Vector_InsertWideStringPair` is descriptive.
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
struct CKeyPairCompareLess {                            // empty, passed by value
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};

namespace std {
struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class BidirectionalIter1, class BidirectionalIter2, class Distance>
BidirectionalIter2 __fastcall __copy_backward(BidirectionalIter1 first, BidirectionalIter1 last, BidirectionalIter2 result,
                                              const random_access_iterator_tag&, Distance*);           // 0x006BEAA0

template <class BidirectionalIter1, class BidirectionalIter2>
inline BidirectionalIter2 copy_backward(BidirectionalIter1 first, BidirectionalIter1 last, BidirectionalIter2 result)
{
    return __copy_backward(first, last, result, random_access_iterator_tag(), (int*)0);
}

template <class RandomAccessIter, class T, class Compare>
void __fastcall __unguarded_linear_insert(RandomAccessIter last, T val, Compare comp);                 // 0x006BE930

template <class RandomAccessIter, class T, class Compare>
void __fastcall __linear_insert(RandomAccessIter first, RandomAccessIter last, T val, Compare comp)
{
    if (comp(val, *first)) {
        copy_backward(first, last, last + 1);
        *first = val;
    }
    else
        __unguarded_linear_insert(last, val, comp);
}

template void __fastcall __linear_insert<CMusicEntryPair*, CMusicEntryPair, CKeyPairCompareLess<long, CMusicManager::CRegisteredMusicEntry> >(
    CMusicEntryPair*, CMusicEntryPair*, CMusicEntryPair, CKeyPairCompareLess<long, CMusicManager::CRegisteredMusicEntry>);
}