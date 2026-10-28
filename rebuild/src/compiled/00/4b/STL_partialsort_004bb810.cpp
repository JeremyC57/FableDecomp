// std::__partial_sort(pair<CCharString, CPersistInfo>* first, pair<CCharString, CPersistInfo>* middle, pair<CCharString, CPersistInfo>* last, pair<CCharString, CPersistInfo>*, CKeyPairCompareLess comp) -- STLport partial sort of the quest manager's persistence table by quest name: heapify the prefix, pop every tail entry whose name sorts before the heap top into it, then heap-sort the prefix
// Leaf: ??$__partial_sort@
// Retail: Fable.exe (TLC, VC7.1) @ 0x004BB810, 152 bytes, `ret 0Ch` (/Gr algorithm TU, explicit __fastcall here: first in
//   ecx, middle in edx, then last, the value-type tag pointer and the empty comparator slot).
// Callers: 0x004BB9BC (partial_sort front end) and the depth-limit fallback of the __introsort_loop 0x004BBA40
//   (0x004BBB2B).
// Callees: __make_heap 0x004BAC90 (catalog "std::Heapify"), __pop_heap 0x004BABF0 (landed; out of line for this element),
//   sort_heap 0x004BB570 (landed), CBasicString<char>::operator< 0x00429950 (behind the inline CCharString::operator<),
//   CCharString copy constructor 0x0099EC30 (x3: the by-value T(*i) built in the argument area).
// Identity: CVectorMap<CCharString, NQuestManager::CRuntimeQuestPersistence::CPersistInfo> entries (12 bytes, see
//   0x004BAFB0); the catalog's "std::Introsort_Recursive" is descriptive.
// Shape: STLport 4.x `make_heap(first, middle, comp); for (i = middle; i < last; ++i) if (comp(*i, *first))
//   __pop_heap(first, middle, i, T(*i), comp, (int*)0); sort_heap(first, middle, comp);` with make_heap inlined. The
//   key compare is the inline CCharString::operator< (shared-block pointer tests, then the real CBasicString compare,
//   kept as a same-TU noinline body); first is spilled to the `push ecx` frame word and both first and the comparator
//   slot are reloaded after the out-of-line __pop_heap.
template <class T> class CBasicString {
public:
    T* m_pData;
#pragma auto_inline(off)
    // 0x00429950 (landed): the real body as a same-TU noinline function (the STLport-over-CCharString rule).
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
    CBasicString<char>* PStringData;                    // the shared string block
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
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

namespace NQuestManager {
class CRuntimeQuestPersistence {
public:
    class CPersistInfo {                                // Ego_r.pdb, 8 bytes (implicit copy / assignment)
    public:
        CCharString QuestName;                          // +0x00
        CCharString PersistScript;                      // +0x04
    };
};
}

namespace std {
template <class _T1, class _T2> struct pair {          // STLport pair: user constructors, implicit operator=
    _T1 first;                                          // +0x00
    _T2 second;                                         // +0x04
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};
}

typedef NQuestManager::CRuntimeQuestPersistence::CPersistInfo TPersistInfo;
typedef std::pair<CCharString, TPersistInfo> TPersistPair;                 // 12 bytes

template <class K, class V> class CKeyPairCompareLess { // Ego_r.pdb: empty, passed by value (4-byte slot)
public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};
typedef CKeyPairCompareLess<CCharString, TPersistInfo> TPersistLess;
namespace std {
template <class _RandomAccessIterator, class _Compare, class _Tp, class _Distance>
void __fastcall __make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp, _Tp*, _Distance*);   // 0x004BAC90
template <class _RandomAccessIterator, class _Tp, class _Compare, class _Distance>
void __fastcall __pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _RandomAccessIterator __result,
                           _Tp __value, _Compare __comp, _Distance*);   // 0x004BABF0 (landed)
template <class _RandomAccessIterator, class _Compare>
void __fastcall sort_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp);   // 0x004BB570 (landed)

template <class _Tp>
inline _Tp* __value_type(const _Tp*) { return (_Tp*)0; }
template <class _Tp>
inline int* __distance_type(const _Tp*) { return (int*)0; }

template <class _RandomAccessIterator, class _Compare>
inline void make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp)
{
    __make_heap(__first, __last, __comp, __value_type(__first), __distance_type(__first));
}

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last, _Tp*, _Compare __comp)
{
    make_heap(__first, __middle, __comp);
    for (_RandomAccessIter __i = __middle; __i < __last; ++__i)
        if (__comp(*__i, *__first))
            __pop_heap(__first, __middle, __i, _Tp(*__i), __comp, __distance_type(__first));
    sort_heap(__first, __middle, __comp);
}
}

template void __fastcall std::__partial_sort<TPersistPair*, TPersistPair, TPersistLess>(TPersistPair*, TPersistPair*, TPersistPair*, TPersistPair*, TPersistLess);