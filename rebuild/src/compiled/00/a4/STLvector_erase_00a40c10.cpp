// std::vector::erase(SNamePairEntry* first, SNamePairEntry* last) -- STLport vector<pair<pair<CCharString, CCharString>, CSoundBankLH*>>::erase(range): copy the tail down over the hole, destroy the leftover entries, shrink _M_finish, return first
// Leaf: ?erase@
// Retail: Fable.exe @ 0x00A40C10, 84 bytes, thiscall `ret 8`.
// Callee: std::__copy<SNamePairEntry*, SNamePairEntry*, int> 0x00A40040 (fastcall: last, _M_finish; result = first, the
//   random_access_iterator_tag reference and the null distance pointer pushed) -- retail homes the empty tag in first's
//   LIVE argument slot (`lea eax,[esp+18h]` after the push 0), so __copy gets its real STLport body here as a same-TU
//   noinline function that never touches the tag. ~CCharString 0x0099EAE0 (inline destroy loop, +4 then +0, stride 12).
// Label: catalog "Vector_Copy_CCharStringPair" is descriptive.
// Element: CSoundManagerLH's NamePairMap entry pair<pair<CCharString, CCharString>, CSoundBankLH*> (12 bytes; the landed
//   FindByNamePair 0x00A3EE50); assignment and destruction implicit.
// Shape: STLport 4.6 `pointer i = copy(last, _M_finish, first); _Destroy(i, _M_finish); _M_finish = i; return first;`
//   with copy -> __copy(..., random_access_iterator_tag(), (int*)0) and the __false_type destroy loop inline.

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
#pragma auto_inline(off)
// 0x00A40040: STLport __copy for random-access iterators (same-TU noinline body; it never reads the tag)
template <class _RandomAccessIter, class _OutputIter, class _Distance>
__declspec(noinline) _OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                                                   const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}
#pragma auto_inline(on)
template <class _InputIter, class _OutputIter>
inline _OutputIter copy(_InputIter __first, _InputIter __last, _OutputIter __result)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for ( ; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _ForwardIterator>
inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    __destroy_aux(__first, __last, __false_type());
}
template <class _Tp> class allocator { public: allocator() { } };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    _Tp* _M_start;                                          // +0x00
    _Tp* _M_finish;                                         // +0x04
    _Tp* _M_end_of_storage;                                 // +0x08
    _Tp* erase(_Tp* __first, _Tp* __last)
    {
        _Tp* __i = copy(__last, _M_finish, __first);
        _Destroy(__i, _M_finish);
        _M_finish = __i;
        return __first;
    }
};
}

template SNamePairEntry* std::vector<SNamePairEntry>::erase(SNamePairEntry*, SNamePairEntry*);