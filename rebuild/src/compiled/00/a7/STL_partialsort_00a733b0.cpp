// _STL::__partial_sort(SStringKeyedChunk* first, SStringKeyedChunk* middle, SStringKeyedChunk* last, SStringKeyedChunk*, CStringKeyedChunkLess comp) -- STLport partial sort of the 16-byte (CCharString key, {name, counted chunk}) records: heap [first, middle), pop every later record with a smaller key into it, then sort the heap
// Leaf: ??$__partial_sort@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A733B0, 149 bytes, `ret 0Ch`. /Gr algorithm TU: first in ecx, middle in edx;
//   last, the null value-type pointer and the empty comparator's 4-byte slot on the stack.
// Callees: __make_heap 0x00A71D40 (behind the inline make_heap), __pop_heap 0x00A71BE0 (`ret 1Ch`), sort_heap 0x00A72B10
//   (landed), CBasicString<char>::operator< 0x00429950 (behind the inline CCharString::operator<), CCharString copy
//   constructor 0x0099EC30 (x2, the value copy).
// Callers: the introsort loop 0x00A73A00 (depth-limit fallback) and partial_sort 0x00A7375C.
// Label: catalog "Introsort_QuickPartition_FloatStr" is descriptive; the body is STLport _algo.c `__partial_sort`.
// Element: pair<CCharString, {CCharString, CCountedPointer}> -- the string-keyed chunk record of the landed 0x00A73840 /
//   0x00A73530 insert (16-byte stride) and of the landed sort_heap 0x00A72B10.
// Shape: STLport 4.6 verbatim; the comparator is CKeyPairCompareLess over the inline CCharString::operator< (same
//   block -> false, null left -> true, null right -> false, else the data compare), the value copy is pair's explicit
//   copy ctor (key string copy, then the record's implicit copy: string copy, counted pointer inline).
template <class T> class CBasicString {
public:
    T* m_pData;
#pragma auto_inline(off)
    // 0x00429950 (landed): the real body as a same-TU noinline function (STLport-over-CCharString rule).
    __declspec(noinline) bool operator<(const CBasicString& other) const
    {
        const T* b = other.m_pData;
        const T* a = m_pData;
        for (;;) {
            if (*a == 0 && *b == 0)
                return false;
            if (*a < *b)
                return true;
            if (*a > *b)
                return false;
            ++a;
            ++b;
        }
    }
#pragma auto_inline(on)
};

class CCharString {
public:
    CBasicString<char>* PStringData;
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
    bool operator<(const CCharString& other) const
    {
        if (other.PStringData == PStringData)
            return false;
        if (PStringData == 0)
            return true;
        if (other.PStringData == 0)
            return false;
        return *PStringData < *other.PStringData;
    }
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

namespace _STL {
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
    bool operator()(const _STL::pair<K, V>& a, const _STL::pair<K, V>& b) const { return a.first < b.first; }
};

typedef _STL::pair<CCharString, SNamedChunk>          SStringKeyedChunk;      // 0x10 bytes
typedef CKeyPairCompareLess<CCharString, SNamedChunk> CStringKeyedChunkLess;

namespace _STL {
template <class _RandomAccessIterator, class _Compare, class _Tp, class _Distance>
void __fastcall __make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp, _Tp*, _Distance*);   // 0x00A71D40
template <class _Tp> inline _Tp* __value_type_of(_Tp*) { return (_Tp*)0; }
template <class _Tp> inline int* __distance_type_of(_Tp*) { return (int*)0; }
template <class _RandomAccessIterator, class _Compare>
inline void make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp)
{
    __make_heap(__first, __last, __comp, __value_type_of(__first), __distance_type_of(__first));
}
template <class _RandomAccessIterator, class _Tp, class _Compare, class _Distance>
void __fastcall __pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _RandomAccessIterator __result,
                           _Tp __val, _Compare __comp, _Distance*);   // 0x00A71BE0
template <class _RandomAccessIterator, class _Compare>
void __fastcall sort_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp);   // 0x00A72B10

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

template void __fastcall _STL::__partial_sort<SStringKeyedChunk*, SStringKeyedChunk, CStringKeyedChunkLess>(SStringKeyedChunk*, SStringKeyedChunk*, SStringKeyedChunk*, SStringKeyedChunk*, CStringKeyedChunkLess);