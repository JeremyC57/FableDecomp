// _STL::__pop_heap(first, last, result, val, comp, int*) -- STLport heap pop for the registered-music map: move the top entry to *result, then sift the by-value val down from the root of the shrunk heap
// Leaf: ??$__pop_heap@
// Retail: Fable.exe (TLC, VC7.1) @ 0x006BEDC0, 179 bytes (+ padding to 192), `ret 24h` (__fastcall /Gr algorithm TU:
//   first in ecx -> ebx, last in edx -> ebp; result, the 24-byte by-value pair, the empty comparator and the null distance
//   pointer on the stack).
// Identity (label correction): the catalog's "pair<long,CMusicManager::CRegisteredMusicEntry>" names only the element. The
//   body is STLport `__pop_heap` over pair<long, CMusicManager::CRegisteredMusicEntry> (24 bytes; Ego_r.pdb
//   CRegisteredMusicEntry: SrcFileName +0, DstFileName +4 (CWideString), Gain +8, Offset +0xC, CacheToHD +0x10), in the
//   same TU as the landed __adjust_heap 0x006BEBF0, __make_heap 0x006BEE80 and __push_heap 0x006BE9D0 of that element.
// Callees: CWideString::operator= @ 0x0099B7D0 (twice), CWideString::CWideString(const CWideString&) @ 0x0099B720 (twice),
//   __adjust_heap(first, 0, last - first, val, comp) @ 0x006BEBF0, CWideString::~CWideString @ 0x0099B510 (twice: val).
// Shape: STLport 4.6 `*__result = *__first; __adjust_heap(__first, _Distance(0), _Distance(__last - __first), __val,
//   __comp);` -- the pair's and the entry's implicit assignments inline (key, the two strings through operator=, then the
//   two floats and the flag); the by-value copy of val is built in the argument area by the pair's user copy constructor
//   over the entry's memberwise copy constructor; val's strings are destroyed on exit (the callee owns its parameter).
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

namespace _STL {
template <class _T1, class _T2> struct pair {
    _T1 first;                                          // +0x00
    _T2 second;                                         // +0x04
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};
}

typedef _STL::pair<long, CMusicManager::CRegisteredMusicEntry> CMusicEntryPair;   // 24 bytes

template <class K, class V> struct CKeyPairCompareLess {   // empty, passed by value
    bool operator()(const _STL::pair<K, V>& a, const _STL::pair<K, V>& b) const { return a.first < b.first; }
};
typedef CKeyPairCompareLess<long, CMusicManager::CRegisteredMusicEntry> TMusicEntryLess;

namespace _STL {
template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __len, _Tp __val,
                              _Compare __comp);         // 0x006BEBF0

template <class _RandomAccessIterator, class _Tp, class _Compare, class _Distance>
void __fastcall __pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _RandomAccessIterator __result,
                           _Tp __val, _Compare __comp, _Distance*)
{
    *__result = *__first;
    __adjust_heap(__first, _Distance(0), _Distance(__last - __first), __val, __comp);
}

template void __fastcall __pop_heap(CMusicEntryPair*, CMusicEntryPair*, CMusicEntryPair*, CMusicEntryPair, TMusicEntryLess, int*);
}