// std::vector::push_back(const SNameEntry& x) -- STLport vector<pair<CCharString, CSoundBankLH*>>::push_back: copy-construct at _M_finish, or grow through _M_insert_overflow
// Leaf: ?push_back@
// Retail: Fable.exe @ 0x00A418A0, 72 bytes (+ int3 padding), thiscall `ret 4`.
// Callee: vector<SNameEntry>::_M_insert_overflow @ 0x00A41480 -- given a same-TU noinline STAND-IN body here (not the
//   real grow code; only its call matters) so the empty __false_type temporary shares x's argument slot as in retail
//   (`lea ecx,[esp+14h]` after the two pushes = the x slot, pushed as the tag reference).
// Label: catalog "CCharStringPair_AppendToVector" is descriptive; this is the STLport member.
// Element: CSoundManagerLH's NameMap entry pair<CCharString, CSoundBankLH*> (8 bytes; the landed FindByName 0x00A3ED30)
//   with STLport's explicit pair copy constructor (CCharString copy ctor 0x0099EC30 + the pointer).
// Shape: STLport 4.6 `if (_M_finish != _M_end_of_storage) { _Construct(_M_finish, x); ++_M_finish; } else
//   _M_insert_overflow(_M_finish, x, _IsPODType(), 1UL, true);`.

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
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> class allocator { public: allocator() { } };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    _Tp* _M_start;                                          // +0x00
    _Tp* _M_finish;                                         // +0x04
    _Tp* _M_end_of_storage;                                 // +0x08
#pragma auto_inline(off)
    // 0x00A41480 (separate function; same-TU stand-in so the empty __false_type temporary shares a dead argument slot)
    __declspec(noinline) void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, unsigned int __fill_len, bool __atend)
    {
        if (__atend && __fill_len == 1 && __position == _M_finish)
            _Construct(_M_start, __x);
    }
#pragma auto_inline(on)
    void push_back(const _Tp& __x)
    {
        if (_M_finish != _M_end_of_storage) {
            _Construct(_M_finish, __x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, __x, __false_type(), 1UL, true);
    }
};
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

template void std::vector<SNameEntry>::push_back(const SNameEntry&);