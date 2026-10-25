// std::__partial_sort(SKeyedChunk* first, SKeyedChunk* middle, SKeyedChunk* last, SKeyedChunk*, CKeyedChunkLess comp) -- STLport partial sort of the 16-byte (long key, {name, counted chunk}) records: heap [first, middle), pop every later record with a smaller key into it, then sort the heap
// Leaf: ??$__partial_sort@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A73330, 123 bytes (+ int3 padding to 128), `ret 0Ch`. /Gr algorithm TU: first in
//   ecx, middle in edx; last, the null value-type pointer and the empty comparator's 4-byte slot on the stack.
// Callees: __make_heap 0x00A71CA0 (behind the inline make_heap: comp, (T*)0, (Distance*)0 pushed), __pop_heap 0x00A71B40
//   (landed; `ret 1Ch`: result, the 16-byte value, comp, the distance pointer), sort_heap 0x00A72AA0 (landed),
//   CCharString copy constructor 0x0099EC30 (the value copy).
// Callers: the introsort loop 0x00A73910 (depth-limit fallback) and partial_sort 0x00A7373C.
// Label: catalog "Introsort_QuickPartition_LongUnsigned" is descriptive; the body is STLport _algo.c `__partial_sort`.
// Element: pair<long, {CCharString, CCountedPointer}> -- the long-keyed chunk record of the landed 0x00A737B0 /
//   0x00A734D0 push_back (16-byte stride), ordered by the signed key (`cmp ; jge`).
// Shape: STLport 4.6 verbatim -- `make_heap(first, middle, comp); for (i = middle; i < last; ++i) if (comp(*i, *first))
//   __pop_heap(first, middle, i, _Tp(*i), comp, __distance_type(first)); sort_heap(first, middle, comp);`. The
//   comparator is inline (the key already in ecx is reused as the first word of the value copy); the value copy is
//   pair's explicit copy ctor: key word, then the record's implicit copy through its own address (string copy call,
//   counted pointer copy inline: both words, `test ; je ; inc [count]`).
class CCharString {
public:
    void* PStringData;
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};

struct SCountBlock {
    long m_references;                                  // +0
    void (__fastcall* m_pfnDestroy)(void*);             // +4
    void* m_pObject;                                    // +8
};

template <class T>
class CCountedPointer {
public:
    T* m_pObject;                                       // +0
    SCountBlock* m_pCount;                              // +4
    CCountedPointer(const CCountedPointer& other) : m_pObject(other.m_pObject), m_pCount(other.m_pCount)
    {
        if (m_pCount) ++m_pCount->m_references;
    }
};

class C3DAnimFileRootChunk;

struct SNamedChunk {                                    // 12 bytes
    CCharString                             Name;       // +0x00
    CCountedPointer<C3DAnimFileRootChunk>   Chunk;      // +0x04
};

namespace std {
template <class _T1, class _T2> class pair {
public:
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};
}

template <class K, class V> class CKeyPairCompareLess {
public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};

typedef std::pair<long, SNamedChunk>          SKeyedChunk;    // 0x10 bytes
typedef CKeyPairCompareLess<long, SNamedChunk> CKeyedChunkLess;

namespace std {
template <class _RandomAccessIterator, class _Compare, class _Tp, class _Distance>
void __fastcall __make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp, _Tp*, _Distance*);   // 0x00A71CA0
template <class _Tp> inline _Tp* __value_type_of(_Tp*) { return (_Tp*)0; }
template <class _Tp> inline int* __distance_type_of(_Tp*) { return (int*)0; }
template <class _RandomAccessIterator, class _Compare>
inline void make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp)
{
    __make_heap(__first, __last, __comp, __value_type_of(__first), __distance_type_of(__first));
}
template <class _RandomAccessIterator, class _Tp, class _Compare, class _Distance>
void __fastcall __pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _RandomAccessIterator __result,
                           _Tp __val, _Compare __comp, _Distance*);   // 0x00A71B40
template <class _RandomAccessIterator, class _Compare>
void __fastcall sort_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp);   // 0x00A72AA0

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last, _Tp*, _Compare __comp)
{
    make_heap(__first, __middle, __comp);
    for (_RandomAccessIter __i = __middle; __i < __last; ++__i)
        if (__comp(*__i, *__first))
            __pop_heap(__first, __middle, __i, _Tp(*__i), __comp, __distance_type_of(__first));
    sort_heap(__first, __middle, __comp);
}
}

template void __fastcall std::__partial_sort<SKeyedChunk*, SKeyedChunk, CKeyedChunkLess>(SKeyedChunk*, SKeyedChunk*, SKeyedChunk*, SKeyedChunk*, CKeyedChunkLess);