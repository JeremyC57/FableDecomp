// std::__insertion_sort(SNameEntry* first, SNameEntry* last, CNameLess comp) -- STLport insertion sort over the 8-byte (name, object) entries: every element after the first goes through __linear_insert(first, i, *i, comp)
// Leaf: ??$__insertion_sort@
// Retail: Fable.exe @ 0x00A40EF0, 66 bytes, `ret 4`. /Gr TU written as explicit __fastcall: first in ecx, last in edx,
//   the comparator slot on the stack.
// Callees: CCharString copy ctor 0x0099EC30 (the by-value *i built in the argument area), __linear_insert 0x00A40730.
// Label: catalog "Vector_Shrink_Destroy_CCharString" is descriptive and wrong.
// Element: CSoundManagerLH's NameMap entry pair<CCharString, CSoundBankLH*> (8 bytes; the landed FindByName 0x00A3ED30),
//   ordered by CKeyPairCompareLess (the key string's inline operator<: same block, null left, null right, then
//   CBasicString<char>::operator< 0x00429950 -- given its landed body as a same-TU noinline function). The pair has
//   STLport's explicit copy constructor; assignment is implicit (CCharString::operator= 0x0099EFB0 + the pointer).
// Shape: STLport 4.6 _algo.c (`if (first == last) return; for (i = first + 1; i != last; ++i) __linear_insert(first, i,
//   *i, comp);`) -- the value goes by value through the pair's explicit copy constructor.

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
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __linear_insert(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __val, _Compare __comp);   // 0x00A40730

template <class _RandomAccessIter, class _Compare>
void __fastcall __insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp)
{
    if (__first == __last)
        return;
    for (_RandomAccessIter __i = __first + 1; __i != __last; ++__i)
        __linear_insert(__first, __i, *__i, __comp);
}
}

template void __fastcall std::__insertion_sort<SNameEntry*, CNameLess>(SNameEntry*, SNameEntry*, CNameLess);