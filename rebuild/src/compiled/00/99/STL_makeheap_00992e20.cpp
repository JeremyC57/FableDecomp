// std::__make_heap(CSoundBankEntry* first, CSoundBankEntry* last, CSoundBankEntryLess comp, CSoundBankEntry*, int*) -- heapify [first, last) by sifting down every parent from the last one to the root
// Leaf: ??$__make_heap@PAU?$pair@JVCSoundBankRecord@@@std@@
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00992E20, length 182 bytes, `ret 0Ch`.
// Callees: CCharString copy ctor @ 0x0099EC30 (x4), __adjust_heap @ 0x00992B80 (landed, by-value entry and comparator).
// Label: the manifest's "std_make_heap_Impl" is descriptive; this is the STLport sort/uninitialized-memory instantiation.
// Element: std::pair<long, CSoundBankRecord> (0x24 bytes) -- the entries of the CASoundManager's bank vector map (sorted by
// the landed CVectorMap::Sort 0x00993A10 -> sort 0x009939B0); record = two CCountedPointers (copy bumps the shared count)
// and four CCharStrings (copy ctor 0x0099EC30), with the out-of-line operator= 0x00992630 and destructor 0x00992510
// (landed). The record's copy constructor is implicit: VC7.1 expands it inline except where it copies a by-value
// parameter (there it calls the out-of-line copy, retail 0x009922F0). The sound module is absent from the Ego PDB, so the
// record / comparator names are descriptive (as the landed siblings 0x00992940 / 0x00992B80); the algorithm names are
// STLport 4.6's. /Gr: retail's link-time __fastcall (first two arguments in ecx/edx), as the landed siblings.
// Shape: STLport 4.6 __make_heap: `len < 2` early out, parent = (len - 2) / 2 (signed `cdq ; sub ; sar`), the loop
// `__adjust_heap(first, parent, len, T(*(first + parent)), comp)` with the copy expanded inline in the outgoing slot.
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

struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };

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

template <class T1, class T2> inline void _Construct(T1* p, const T2& value) { new (p) T1(value); }

template <class InputIter, class ForwardIter>
ForwardIter __uninitialized_copy(InputIter first, InputIter last, ForwardIter result, const __false_type&)
{
    ForwardIter cur = result;
    for (; first != last; ++first, ++cur)
        _Construct(&*cur, *first);
    return cur;
}

template <class ForwardIter, class Size, class T>
ForwardIter __uninitialized_fill_n(ForwardIter first, Size n, const T& x, const __false_type&)
{
    ForwardIter cur = first;
    for (; n > 0; --n, ++cur)
        _Construct(&*cur, x);
    return cur;
}

template <class BidirectionalIter1, class BidirectionalIter2, class Distance>
BidirectionalIter2 __copy_backward(BidirectionalIter1 first, BidirectionalIter1 last, BidirectionalIter2 result,
                                   const random_access_iterator_tag&, Distance*);   // 0x009929C0

template <class BidirectionalIter1, class BidirectionalIter2>
inline BidirectionalIter2 copy_backward(BidirectionalIter1 first, BidirectionalIter1 last, BidirectionalIter2 result)
{
    return __copy_backward(first, last, result, random_access_iterator_tag(), (ptrdiff_t*)0);
}

template <class ForwardIter1, class ForwardIter2, class T>
inline void __iter_swap(ForwardIter1 a, ForwardIter2 b, T*)
{
    T tmp = *a;
    *a = *b;
    *b = tmp;
}

template <class ForwardIter1, class ForwardIter2>
inline void iter_swap(ForwardIter1 a, ForwardIter2 b)
{
    __iter_swap(a, b, (CSoundBankEntry*)0);
}

template <class RandomAccessIter, class T, class Compare>
void __unguarded_linear_insert(RandomAccessIter last, T val, Compare comp)
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

template <class RandomAccessIter, class T, class Compare>
inline void __linear_insert(RandomAccessIter first, RandomAccessIter last, T val, Compare comp)
{
    if (comp(val, *first)) {
        copy_backward(first, last, last + 1);
        *first = val;
    }
    else
        __unguarded_linear_insert(last, val, comp);
}

template <class RandomAccessIter, class Compare>
void __insertion_sort(RandomAccessIter first, RandomAccessIter last, Compare comp)
{
    if (first == last) return;
    for (RandomAccessIter i = first + 1; i != last; ++i)
        __linear_insert(first, i, CSoundBankEntry(*i), comp);
}

template <class RandomAccessIter, class T, class Compare>
void __unguarded_insertion_sort_aux(RandomAccessIter first, RandomAccessIter last, T*, Compare comp)
{
    for (RandomAccessIter i = first; i != last; ++i)
        __unguarded_linear_insert(i, T(*i), comp);
}

template <class RandomAccessIter, class T, class Compare>
RandomAccessIter __unguarded_partition(RandomAccessIter first, RandomAccessIter last, T pivot, Compare comp)
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

template <class RandomAccessIter, class Distance, class T, class Compare>
void __adjust_heap(RandomAccessIter first, Distance holeIndex, Distance len, T value, Compare comp);   // 0x00992B80

template <class RandomAccessIter, class Compare, class T, class Distance>
void __make_heap(RandomAccessIter first, RandomAccessIter last, Compare comp, T*, Distance*)
{
    if (last - first < 2) return;
    Distance len = last - first;
    Distance parent = (len - 2) / 2;
    for (;;) {
        __adjust_heap(first, parent, len, T(*(first + parent)), comp);
        if (parent == 0) return;
        parent--;
    }
}

}   // namespace std

template void std::__make_heap(CSoundBankEntry*, CSoundBankEntry*, CSoundBankEntryLess, CSoundBankEntry*, int*);