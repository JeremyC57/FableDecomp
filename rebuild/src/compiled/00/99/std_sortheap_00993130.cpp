// std::sort_heap(CSoundBankEntry* first, CSoundBankEntry* last, CSoundBankEntryLess comp) -- order a heap of (long key, sound-bank record) pairs by repeatedly popping the top to the end
// Leaf: ??$sort_heap@PAU?$pair@JVCSoundBankRecord@@@std@@
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00993130, length 382 bytes, `ret 4`.
// Callees: CCharString copy ctor @ 0x0099EC30 (x8), the record's operator= @ 0x00992630, __adjust_heap @ 0x00992B80,
// the record's destructor @ 0x00992510 (all landed).
// Label correction: the manifest's _Sort_heap<pair<unsigned long, C3DHelperPoint>*, CKeyPairCompareLess<...>> is wrong:
// C3DHelperPoint is 16 bytes (Ego PDB) and this element is 0x24 bytes (`imul 0x38E38E39 ; sar 3`), the
// std::pair<long, CSoundBankRecord> of the CASoundManager bank map whose __adjust_heap (0x00992B80) and __make_heap
// (0x00992E20) are landed; the caller 0x00993634 is the same map's sort.
// Shape: STLport 4.6 sort_heap: `while (last - first > 1) pop_heap(first, last--, comp)` with pop_heap -> __pop_heap_aux ->
// __pop_heap inlined: T(*(last - 1)) built in a frame temporary (implicit copies expanded inline), *(last - 1) = *first
// (key store + out-of-line record operator=), __adjust_heap(first, 0, (last - 1) - first, value, comp) with the by-value
// copy built inline in the outgoing slot, then the temporary's record destroyed out of line.
#include <new>
#include <stddef.h>

class CCharStringData;
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString(const CCharString& other);      // 0x0099EC30
    ~CCharString();                             // 0x0099EAE0
};

class CCPPointerInfo {
public:
    long RefCount;                              // +0x00
};

template <class T> class CCountedPointer {
public:
    T*              Data;                       // +0x00
    CCPPointerInfo* Info;                       // +0x04
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
};

class CSoundBank;
class CSoundBankData;

// The 0x20-byte sound-bank record (descriptive; see the landed ~CSoundBankRecord @ 0x00992510): implicit copy
// constructor, out-of-line assignment and destructor.
class CSoundBankRecord {
public:
    CCountedPointer<CSoundBank>     Bank;       // +0x00
    CCountedPointer<CSoundBankData> BankData;   // +0x08
    CCharString Name0;                          // +0x10
    CCharString Name1;                          // +0x14
    CCharString Name2;                          // +0x18
    CCharString Name3;                          // +0x1c
    CSoundBankRecord& operator=(const CSoundBankRecord& other);   // 0x00992630
    ~CSoundBankRecord();                        // 0x00992510
};

namespace std {

template <class T1, class T2> struct pair {
    typedef T1 first_type;
    typedef T2 second_type;
    T1 first;                                   // +0x00
    T2 second;                                  // +0x04
    pair(const T1& a, const T2& b) : first(a), second(b) { }
};

}   // namespace std

typedef std::pair<long, CSoundBankRecord> CSoundBankEntry;     // 0x24 bytes

// CVectorMap<long, CSoundBankRecord>'s key order (an empty functor passed by value)
struct CSoundBankEntryLess {
    bool operator()(const CSoundBankEntry& a, const CSoundBankEntry& b) const { return a.first < b.first; }
};

namespace std {

template <class RandomAccessIter, class Distance, class T, class Compare>
void __adjust_heap(RandomAccessIter first, Distance holeIndex, Distance len, T value, Compare comp);   // 0x00992B80

template <class RandomAccessIter, class T, class Compare, class Distance>
inline void __pop_heap(RandomAccessIter first, RandomAccessIter last, RandomAccessIter result, T val, Compare comp,
                       Distance*)
{
    *result = *first;
    __adjust_heap(first, Distance(0), Distance(last - first), val, comp);
}

template <class RandomAccessIter, class T, class Compare>
inline void __pop_heap_aux(RandomAccessIter first, RandomAccessIter last, T*, Compare comp)
{
    __pop_heap(first, last - 1, last - 1, T(*(last - 1)), comp, (ptrdiff_t*)0);
}

template <class RandomAccessIter, class Compare>
inline void pop_heap(RandomAccessIter first, RandomAccessIter last, Compare comp)
{
    __pop_heap_aux(first, last, (CSoundBankEntry*)0, comp);
}

template <class RandomAccessIter, class Compare>
void sort_heap(RandomAccessIter first, RandomAccessIter last, Compare comp)
{
    while (last - first > 1)
        pop_heap(first, last--, comp);
}

}   // namespace std

template void std::sort_heap(CSoundBankEntry*, CSoundBankEntry*, CSoundBankEntryLess);