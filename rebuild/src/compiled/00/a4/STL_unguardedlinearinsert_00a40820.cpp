// std::__unguarded_linear_insert(SNamePairEntry* last, SNamePairEntry val, CNamePairLess comp) -- STLport insertion-sort inner step over the 12-byte ((name, name), object) entries: shift larger entries up until val's slot is found, then store val
// Leaf: ??$__unguarded_linear_insert@
// Retail: Fable.exe @ 0x00A40820, 189 bytes (+ int3 padding), `ret 10h`. /Gr TU written as explicit __fastcall: last in
//   ecx, the by-value val (12 bytes) and the comparator slot on the stack; the callee destroys val's two strings.
// Callers: __linear_insert 0x00A40F50 (and the unguarded insertion sort).
// Label: catalog "Vector_Insert_Triple_CCharString" is descriptive.
// Element: CSoundManagerLH's NamePairMap entry pair<pair<CCharString, CCharString>, CSoundBankLH*> (12 bytes; the landed
//   FindByNamePair 0x00A3EE50), ordered by CKeyPairCompareLess on the (name, name) key = STLport's pair operator<, which
//   is INLINE in this row (CCharString's inline operator< twice, CBasicString<char>::operator< 0x00429950 out of line).
//   Assignment is implicit (CCharString::operator= 0x0099EFB0 twice + pointer).
// Shape: STLport 4.6 _algo.c verbatim (`next = last; --next; while (comp(val, *next)) { *last = *next; last = next;
//   --next; } *last = val;`) with the pair operator< inline in the loop test.

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
// STLport's pair operator< (out of line at 0x00A40250 elsewhere, __cdecl; inlined in this row)
template <class _T1, class _T2>
inline bool __cdecl operator<(const pair<_T1, _T2>& __x, const pair<_T1, _T2>& __y)
{
    return __x.first < __y.first || (!(__y.first < __x.first) && __x.second < __y.second);
}
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
void __fastcall __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp)
{
    _RandomAccessIter __next = __last;
    --__next;
    while (__comp(__val, *__next)) {
        *__last = *__next;
        __last = __next;
        --__next;
    }
    *__last = __val;
}
}

template void __fastcall std::__unguarded_linear_insert<SNamePairEntry*, SNamePairEntry, CNamePairLess>(SNamePairEntry*, SNamePairEntry, CNamePairLess);