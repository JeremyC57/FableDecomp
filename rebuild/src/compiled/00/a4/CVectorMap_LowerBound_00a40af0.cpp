// CVectorMap::LowerBound(const pair<CCharString, CCharString>& key) const -- binary search of the (name, name) -> object map's sorted 12-byte entries for the first key not below `key`
// Leaf: ?LowerBound@
// Retail: Fable.exe @ 0x00A40AF0, 217 bytes (+ int3 padding), thiscall `ret 4`.
// Callers: the inline CVectorMap::Find in CSoundManagerLH::FindByNamePair 0x00A3EE50.
// Label: catalog "Map_FindUpper_CCharStringPair" is descriptive and wrong (lower, not upper bound); LowerBound is
//   CVectorMap's PDB member name (Ego_r carries ?LowerBound@?$CVectorMap@... for other instantiations).
// Element: CSoundManagerLH's NamePairMap entry pair<pair<CCharString, CCharString>, CSoundBankLH*> (12 bytes; the landed
//   FindByNamePair 0x00A3EE50), ordered by CKeyPairCompareLess on the (name, name) key = STLport's pair operator<, which
//   is INLINE in this row (CCharString's inline operator< twice, CBasicString<char>::operator< 0x00429950 out of line).
//   Assignment is implicit (CCharString::operator= 0x0099EFB0 twice + pointer).
// Shape: the landed CVectorMap::LowerBound 0x00447C80 loop (`while (count > 0) { half = count / 2; middle = first + half;
//   if (comp(*middle, key)) { first = middle + 1; count -= half + 1; } else count = half; }`), with `count = _M_finish -
//   _M_start` computed BEFORE `first = _M_start` (first-then-count loads _M_start straight into ebp; retail reads it into
//   esi and copies it). first lives in a frame word, the key's first string block is cached in another, and count <= 0
//   returns at once.

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

template <class K, class V, class C = CKeyPairCompareLess<K, V> > class CVectorMap {   // retail 16 bytes
public:
    typedef std::pair<K, V> value_type;
    value_type* _M_start;                                   // +0x00 (the STLport vector Data)
    value_type* _M_finish;                                  // +0x04
    value_type* _M_end_of_storage;                          // +0x08
    C           Comp;                                       // +0x0C (empty comparator)
    bool        Dirty;                                      // +0x0D
    const value_type* LowerBound(const K& key) const;
};

template <class K, class V, class C>
const typename CVectorMap<K, V, C>::value_type* CVectorMap<K, V, C>::LowerBound(const K& key) const
{
    int count = _M_finish - _M_start;
    const value_type* first = _M_start;
    while (count > 0) {
        int half = count / 2;
        const value_type* middle = first + half;
        if (Comp(*middle, key)) {
            first = middle + 1;
            count -= half + 1;
        }
        else
            count = half;
    }
    return first;
}

template const SNamePairEntry* CVectorMap<SNamePair, CSoundBankLH*>::LowerBound(const SNamePair&) const;