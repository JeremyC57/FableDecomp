// std::__pop_heap(SNamePairEntry* first, SNamePairEntry* last, SNamePairEntry* result, SNamePairEntry val, CNamePairLess comp, int*) -- STLport heap pop over the 12-byte ((name, name), object) entries: move the top to *result and sift val down from the root
// Leaf: ??$__pop_heap@
// Retail: Fable.exe @ 0x00A41630, 131 bytes, `ret 18h`. /Gr TU written as explicit __fastcall: first in ecx, last in
//   edx; result, the by-value val (12 bytes), the comparator slot and the null distance pointer on the stack.
// Callers: __partial_sort 0x00A41CD0 (and sort_heap). Callees: CCharString::operator= 0x0099EFB0 / copy ctor 0x0099EC30
//   / ~CCharString 0x0099EAE0, __adjust_heap 0x00A410A0.
// Label: catalog "_Pop_heap<CTestQuest*,int,CTestQuest,CTestQuestNameCompare>" is a wrong propagated Dinkumware name.
// Element: CSoundManagerLH's NamePairMap entry pair<pair<CCharString, CCharString>, CSoundBankLH*> (12 bytes; the landed
//   FindByNamePair 0x00A3EE50), ordered by CKeyPairCompareLess on the (name, name) key = STLport's pair operator<, which
//   retail calls out of line here (0x00A40250, __cdecl, two pointers). Pair copy constructors are STLport's explicit
//   ones (CCharString copy ctor 0x0099EC30); assignment is implicit (CCharString::operator= 0x0099EFB0 twice + pointer).
// Shape: STLport 4.6 _heap.c verbatim (`*result = *first; __adjust_heap(first, _Distance(0), _Distance(last - first),
//   val, comp);`), val copied into __adjust_heap's argument area, then destroyed.

inline void* operator new(unsigned int, void* p) { return p; }   // placement new (STLport <new>)

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
    CBasicString<char>* PStringData;                        // the shared block (CBasicString<char> first)
    CCharString(const CCharString& other);                  // 0x0099EC30
    ~CCharString();                                         // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);       // 0x0099EFB0
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

class CSoundBankLH;                                         // descriptive: the mapped object of CSoundManagerLH's maps

namespace std {
struct __true_type { __true_type() { } };
struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};
template <class _T1, class _T2>
bool __cdecl operator<(const pair<_T1, _T2>& __x, const pair<_T1, _T2>& __y);   // 0x00A40250 (pair<CCharString, CCharString>)
}

template <class K, class V> class CKeyPairCompareLess {
public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
    bool operator()(const K& a, const std::pair<K, V>& b) const { return a < b.first; }
    bool operator()(const std::pair<K, V>& a, const K& b) const { return a.first < b; }
};

typedef std::pair<CCharString, CCharString>      SNamePair;      // the (name, name) key
typedef std::pair<CCharString, CSoundBankLH*>     SNameEntry;     // NameMap entry, 8 bytes
typedef std::pair<SNamePair, CSoundBankLH*>       SNamePairEntry; // NamePairMap entry, 12 bytes
typedef CKeyPairCompareLess<CCharString, CSoundBankLH*> CNameLess;
typedef CKeyPairCompareLess<SNamePair, CSoundBankLH*>   CNamePairLess;

namespace std {
template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __len, _Tp __val, _Compare __comp);   // 0x00A410A0

template <class _RandomAccessIterator, class _Tp, class _Compare, class _Distance>
void __fastcall __pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _RandomAccessIterator __result,
                           _Tp __val, _Compare __comp, _Distance*)
{
    *__result = *__first;
    __adjust_heap(__first, _Distance(0), _Distance(__last - __first), __val, __comp);
}
}

template void __fastcall std::__pop_heap<SNamePairEntry*, SNamePairEntry, CNamePairLess, int>(SNamePairEntry*, SNamePairEntry*, SNamePairEntry*, SNamePairEntry, CNamePairLess, int*);