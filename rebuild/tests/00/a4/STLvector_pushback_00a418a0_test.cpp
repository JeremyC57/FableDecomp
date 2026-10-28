#include <cstdio>
#include <stdlib.h>
#include <string.h>
#include <new>

static int g_blocks = 0;                                    // live string blocks (leak / double-free check)

template <class T> class CBasicString {
public:
    T* m_pData;
    bool operator<(const CBasicString& other) const
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
};

// Test CCharString: one heap block per non-empty string (null block = empty), copies duplicate the block.
class CCharString {
public:
    CBasicString<char>* PStringData;
    CCharString() : PStringData(0) { }
    explicit CCharString(const char* s) : PStringData(0) { Attach(s); }
    CCharString(const CCharString& other) : PStringData(0) { if (other.PStringData) Attach(other.PStringData->m_pData); }
    ~CCharString() { Detach(); }
    const CCharString& operator=(const CCharString& other)
    {
        if (this != &other) {
            char* s = other.PStringData ? other.PStringData->m_pData : 0;
            Detach();
            if (s)
                Attach(s);
        }
        return *this;
    }
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
    const char* c_str() const { return PStringData ? PStringData->m_pData : ""; }
private:
    void Attach(const char* s)
    {
        PStringData = new CBasicString<char>;
        PStringData->m_pData = (char*)s;
        ++g_blocks;
    }
    void Detach()
    {
        if (PStringData) {
            delete PStringData;
            PStringData = 0;
            --g_blocks;
        }
    }
};

class CSoundBankLH {
public:
    int Id;
};

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
inline bool operator<(const pair<_T1, _T2>& __x, const pair<_T1, _T2>& __y)
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

typedef std::pair<CCharString, CCharString>      SNamePair;
typedef std::pair<CCharString, CSoundBankLH*>     SNameEntry;
typedef std::pair<SNamePair, CSoundBankLH*>       SNamePairEntry;
typedef CKeyPairCompareLess<CCharString, CSoundBankLH*> CNameLess;
typedef CKeyPairCompareLess<SNamePair, CSoundBankLH*>   CNamePairLess;

static CSoundBankLH g_banks[16];

// raw arrays of entries (the pairs have no default constructor)
static SNameEntry* MakeNames(const char* const* names, int n, int capacity)
{
    SNameEntry* p = (SNameEntry*)malloc(capacity * sizeof(SNameEntry));
    for (int i = 0; i < n; ++i)
        new (p + i) SNameEntry(CCharString(names[i]), &g_banks[i]);
    return p;
}
static void FreeNames(SNameEntry* p, int n)
{
    for (int i = 0; i < n; ++i)
        p[i].~SNameEntry();
    free(p);
}
static SNamePairEntry* MakePairs(const char* const* names, int n, int capacity)   // names: n (first, second) pairs
{
    SNamePairEntry* p = (SNamePairEntry*)malloc(capacity * sizeof(SNamePairEntry));
    for (int i = 0; i < n; ++i)
        new (p + i) SNamePairEntry(SNamePair(CCharString(names[2 * i]), CCharString(names[2 * i + 1])), &g_banks[i]);
    return p;
}
static void FreePairs(SNamePairEntry* p, int n)
{
    for (int i = 0; i < n; ++i)
        p[i].~SNamePairEntry();
    free(p);
}
static bool NameIs(const SNameEntry& e, const char* s) { return strcmp(e.first.c_str(), s) == 0; }
static bool PairIs(const SNamePairEntry& e, const char* a, const char* b)
{
    return strcmp(e.first.first.c_str(), a) == 0 && strcmp(e.first.second.c_str(), b) == 0;
}
template <class T, class C> static bool Sorted(const T* p, int n, C comp)
{
    for (int i = 1; i < n; ++i)
        if (comp(p[i], p[i - 1]))
            return false;
    return true;
}
template <class T, class C> static bool IsHeap(const T* p, int n, C comp)
{
    for (int i = 1; i < n; ++i)
        if (comp(p[(i - 1) / 2], p[i]))
            return false;
    return true;
}

namespace std {
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> class allocator { public: allocator() { } };
static int g_overflow = 0;
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    // test stub for the out-of-line grow path 0x00A41480: records the call
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, unsigned int __fill_len, bool __atend)
    {
        if (__atend && __fill_len == 1 && __position == _M_finish && __x.second != 0)
            ++g_overflow;
    }
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

int main()
{
    std::vector<SNameEntry> v;
    v._M_start = v._M_finish = (SNameEntry*)malloc(2 * sizeof(SNameEntry));
    v._M_end_of_storage = v._M_start + 2;
    v.push_back(SNameEntry(CCharString("bravo"), &g_banks[1]));
    v.push_back(SNameEntry(CCharString("alpha"), &g_banks[0]));
    v.push_back(SNameEntry(CCharString("charlie"), &g_banks[2]));
    if (v._M_finish != v._M_start + 2 || !NameIs(v._M_start[0], "bravo") || !NameIs(v._M_start[1], "alpha")
        || v._M_start[1].second != &g_banks[0] || std::g_overflow != 1 || g_blocks != 2)
        { std::printf("00a418a0_TEST FAIL\n"); return 1; }
    FreeNames(v._M_start, 2);
    if (g_blocks != 0)
        { std::printf("00a418a0_TEST FAIL\n"); return 1; }
    std::printf("00a418a0_TEST PASS\n");
    return 0;
}