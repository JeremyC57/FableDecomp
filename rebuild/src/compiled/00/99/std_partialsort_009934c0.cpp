// std::__partial_sort(CSoundBankEntry* first, CSoundBankEntry* middle, CSoundBankEntry* last, CSoundBankEntry*, CSoundBankEntryLess comp) -- STLport partial sort of the sound manager's (long key, sound-bank record) entries: heapify [first, middle), pop every smaller tail element into the heap, then sort the heap
// Leaf: ??$__partial_sort@
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x009934C0, 387 bytes (+ int3 padding), __fastcall (/Gr algorithm TU: first in ecx,
//   middle in edx; last, the _Tp* tag and the empty comparator on the stack), `ret 0Ch`.
// Identity: the catalog's "Heap_BuildFrom" is descriptive. Caller: the same sort's __introsort_loop 0x00993860 (this
//   chunk) when its depth budget runs out (partial_sort(first, last, last, comp) inlined around this call). The element
//   is std::pair<long, CSoundBankRecord> (0x24 bytes) of the CASoundManager bank vector map, as the landed siblings
//   __make_heap 0x00992E20, __adjust_heap 0x00992B80 and sort_heap 0x00993130 model it.
// Callees: __make_heap 0x00992E20, the CCharString copy constructor 0x0099EC30 (x8), the record's operator= 0x00992630,
//   __adjust_heap 0x00992B80, ~CSoundBankRecord 0x00992510, sort_heap 0x00993130 (all landed).
// Shape: STLport 4.6 `__partial_sort` verbatim: make_heap -> `__make_heap(first, middle, comp, (T*)0, (ptrdiff_t*)0)`;
//   the `i < last` loop becomes a counted loop over (last - middle - 1) / 36 + 1 (the count homed in the dead `last`
//   slot); the key compare is inlined (signed `jge`); __pop_heap is inlined: T(*i) in a frame temporary (the record's
//   implicit copy expanded inline), `*i = *first` (key store + out-of-line operator=), the by-value copy for
//   __adjust_heap built in the outgoing slot, then the temporary's record destroyed.
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

template <class RandomAccessIter, class Compare, class T, class Distance>
void __make_heap(RandomAccessIter first, RandomAccessIter last, Compare comp, T*, Distance*);          // 0x00992E20

template <class RandomAccessIter, class Distance, class T, class Compare>
void __adjust_heap(RandomAccessIter first, Distance holeIndex, Distance len, T value, Compare comp);   // 0x00992B80

template <class RandomAccessIter, class Compare>
void sort_heap(RandomAccessIter first, RandomAccessIter last, Compare comp);                         // 0x00993130

template <class RandomAccessIter, class Compare>
inline void make_heap(RandomAccessIter first, RandomAccessIter last, Compare comp)
{
    __make_heap(first, last, comp, (CSoundBankEntry*)0, (int*)0);
}

template <class RandomAccessIter, class T, class Compare, class Distance>
inline void __pop_heap(RandomAccessIter first, RandomAccessIter last, RandomAccessIter result, T val, Compare comp,
                       Distance*)
{
    *result = *first;
    __adjust_heap(first, Distance(0), Distance(last - first), val, comp);
}

template <class RandomAccessIter, class T, class Compare>
void __partial_sort(RandomAccessIter first, RandomAccessIter middle, RandomAccessIter last, T*, Compare comp)
{
    make_heap(first, middle, comp);
    for (RandomAccessIter i = middle; i < last; ++i)
        if (comp(*i, *first))
            __pop_heap(first, middle, i, T(*i), comp, (int*)0);
    sort_heap(first, middle, comp);
}

}   // namespace std

template void std::__partial_sort(CSoundBankEntry*, CSoundBankEntry*, CSoundBankEntry*, CSoundBankEntry*, CSoundBankEntryLess);