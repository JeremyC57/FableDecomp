// std::__introsort_loop(CSoundBankEntry* first, CSoundBankEntry* last, CSoundBankEntry*, int depth_limit, CSoundBankEntryLess comp) -- STLport introsort recursion of the sound manager's (long key, sound-bank record) entries: median-of-three partition until 16 elements remain, partial (heap) sort when the depth budget runs out
// Leaf: ??$__introsort_loop@
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00993860, 336 bytes (+ int3 padding), __fastcall (/Gr algorithm TU: first in ecx,
//   last in edx; the _Tp* tag, the depth limit and the empty comparator on the stack), `ret 0Ch`.
// Identity: the catalog's "Heap_Sift_Down_Recursive" is descriptive. Callers: sort 0x009939B0 (landed; the
//   CVectorMap<long, CSoundBankRecord>::Sort of the CASoundManager bank map) and itself (0x00993968).
// Callees: the CCharString copy constructor 0x0099EC30 (x4: the pivot copy), __unguarded_partition 0x00992CB0 (landed),
//   itself, __partial_sort 0x009934C0 (this chunk; STLport's partial_sort inlined around it).
// Shape: STLport 4.x `__introsort_loop` verbatim (as the landed CThing twin 0x00979760), here with __median INLINED (the
//   key compares on `.first`, signed `jl`/`jge`, select a pointer in esi, `c` the default) and the pivot `_Tp(__median(..))`
//   copy-constructed straight into __unguarded_partition's by-value argument slot (record copy expanded inline:
//   counted pointers bump their counts, four CCharString copies).
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

const int __stl_threshold = 16;

template <class _Tp, class _Compare>
inline const _Tp& __median(const _Tp& __a, const _Tp& __b, const _Tp& __c, _Compare __comp)
{
    if (__comp(__a, __b))
        if (__comp(__b, __c))
            return __b;
        else if (__comp(__a, __c))
            return __c;
        else
            return __a;
    else if (__comp(__a, __c))
        return __a;
    else if (__comp(__b, __c))
        return __c;
    else
        return __b;
}

template <class _RandomAccessIter, class _Tp, class _Compare>
_RandomAccessIter __unguarded_partition(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __pivot,
                                        _Compare __comp);                                             // 0x00992CB0

template <class _RandomAccessIter, class _Tp, class _Compare>
void __partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last, _Tp*,
                    _Compare __comp);                                                                 // 0x009934C0

template <class _RandomAccessIter, class _Compare>
inline void partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last,
                         _Compare __comp)
{
    __partial_sort(__first, __middle, __last, (CSoundBankEntry*)0, __comp);
}

template <class _RandomAccessIter, class _Tp, class _Size, class _Compare>
void __introsort_loop(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Size __depth_limit, _Compare __comp)
{
    while (__last - __first > __stl_threshold) {
        if (__depth_limit == 0) {
            partial_sort(__first, __last, __last, __comp);
            return;
        }
        --__depth_limit;
        _RandomAccessIter __cut =
            __unguarded_partition(__first, __last,
                                  _Tp(__median(*__first, *(__first + (__last - __first) / 2), *(__last - 1), __comp)),
                                  __comp);
        __introsort_loop(__cut, __last, (_Tp*)0, __depth_limit, __comp);
        __last = __cut;
    }
}

}   // namespace std

template void std::__introsort_loop(CSoundBankEntry*, CSoundBankEntry*, CSoundBankEntry*, int, CSoundBankEntryLess);