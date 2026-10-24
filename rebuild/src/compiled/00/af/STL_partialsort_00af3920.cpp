// _STL::__partial_sort(CPSCPair* first, CPSCPair* middle, CPSCPair* last, CPSCPair*, CPSCLess comp) -- STLport partial_sort over the 16-byte registered-PSC entries pair<CCharString, NParticleEngine::CRegisteredPSCInfo>: heapify [first, middle), pop every entry of [middle, last) whose name sorts before the heap top into the heap, then sort the heap
// Leaf: ??$__partial_sort@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AF3920, 141 bytes, `ret 0Ch`. /Gr algorithm TU written as explicit __fastcall: first in
//   ecx, middle in edx, then last, the value-type pointer tag and the 4-byte empty comparator slot on the stack.
// Callers: __introsort_loop 0x00AF3AA0 (the depth-limit fallback, i.e. partial_sort(first, last, last)).
// Callees: __make_heap 0x00AF34E0, __pop_heap 0x00AF3430 (landed; out of line for this element), sort_heap 0x00AF3770
//   (landed), CBasicString<char>::operator< 0x00429950 (behind the inline CCharString::operator<), CCharString copy ctor
//   0x0099EC30 (x2: the by-value T(*i) built in the argument area).
// Element: CPSCManager's RegisteredPSCs entry: key CCharString +0, CRegisteredPSCInfo {AllocFunc, ComponentType, GUIName} +4.
// Label: catalog "HeapSort_Phase" is descriptive.
// Shape: STLport 4.6 _algo.c verbatim (`make_heap(first, middle, comp); for (i = middle; i < last; ++i) if (comp(*i, *first))
//   __pop_heap(first, middle, i, T(*i), comp, (int*)0); sort_heap(first, middle, comp);`); the block compare is a
//   same-TU noinline body, so only the out-of-line __pop_heap forces the comparator reload (`mov esi,[esp+1Ch]`).
class CPSCBase;

template <class T> class CBasicString {
public:
    T* m_pData;
#pragma auto_inline(off)
    // 0x00429950 (landed): the real body as a same-TU noinline function, as the STLport-over-CCharString rule asks.
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
    CBasicString<char>* PStringData;                        // the shared block
    CCharString(const CCharString& other);                  // 0x0099EC30
    ~CCharString();                                         // 0x0099EAE0
    CCharString& operator=(const CCharString& other);       // 0x0099EFB0
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

enum EPSCType { PSC_TYPE_NONE = 0 };

namespace NParticleEngine {
struct CRegisteredPSCInfo {
    CPSCBase* (__cdecl* AllocFunc)();                       // +0x0
    EPSCType ComponentType;                                 // +0x4
    CCharString GUIName;                                    // +0x8
};
}

namespace _STL {
template <class _T1, class _T2> struct pair {
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

typedef _STL::pair<CCharString, NParticleEngine::CRegisteredPSCInfo> CPSCPair;
typedef CKeyPairCompareLess<CCharString, NParticleEngine::CRegisteredPSCInfo> CPSCLess;

namespace _STL {
template <class _RandomAccessIterator, class _Compare, class _Tp, class _Distance>
void __fastcall __make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp, _Tp*, _Distance*);   // 0x00AF34E0
template <class _RandomAccessIterator, class _Tp, class _Compare, class _Distance>
void __fastcall __pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _RandomAccessIterator __result,
                           _Tp __value, _Compare __comp, _Distance*);   // 0x00AF3430 (landed)
template <class _RandomAccessIterator, class _Compare>
void __fastcall sort_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp);   // 0x00AF3770 (landed)

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

template void __fastcall _STL::__partial_sort<CPSCPair*, CPSCPair, CPSCLess>(CPSCPair*, CPSCPair*, CPSCPair*, CPSCPair*, CPSCLess);