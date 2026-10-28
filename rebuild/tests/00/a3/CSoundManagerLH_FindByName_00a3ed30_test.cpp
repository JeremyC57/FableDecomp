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

template <class K, class V, class C = CKeyPairCompareLess<K, V> > class CVectorMap {
public:
    typedef std::pair<K, V> value_type;
    value_type* _M_start;
    value_type* _M_finish;
    value_type* _M_end_of_storage;
    C           Comp;
    bool        Dirty;
    // test version of the out-of-line LowerBound (0x00A404D0 / 0x00A40AF0)
    const value_type* LowerBound(const K& key) const
    {
        int count = (int)(_M_finish - _M_start);
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
    const value_type* End() const { return _M_finish; }
    const value_type* Find(const K& key) const
    {
        const value_type* it = LowerBound(key);
        const value_type* e = _M_finish;
        if (it == e || Comp(key, *it))
            return e;
        return it;
    }
};

class CSoundManagerLH {
public:
    CVectorMap<std::pair<CCharString, CCharString>, CSoundBankLH*> NamePairMap;
    CVectorMap<CCharString, CSoundBankLH*>                          NameMap;
    CSoundBankLH* FindByName(const CCharString& name);
    CSoundBankLH* FindByNamePair(const CCharString& first, const CCharString& second);
};

CSoundBankLH* CSoundManagerLH::FindByName(const CCharString& name)
{
    const CVectorMap<CCharString, CSoundBankLH*>::value_type* it = NameMap.Find(name);
    if (it == NameMap.End())
        return 0;
    return it->second;
}

int main()
{
    const char* names[4] = { "ambient", "music", "speech", "ui" };
    CSoundManagerLH m;
    m.NameMap._M_start = MakeNames(names, 4, 4);
    m.NameMap._M_finish = m.NameMap._M_end_of_storage = m.NameMap._M_start + 4;
    {
        CCharString k1("speech"), k2("sfx"), k3("zzz"), k4;
        if (m.FindByName(k1) != &g_banks[2] || m.FindByName(k2) != 0 || m.FindByName(k3) != 0 || m.FindByName(k4) != 0)
            { std::printf("00a3ed30_TEST FAIL\n"); return 1; }
        CCharString k5("ambient");
        if (m.FindByName(k5) != &g_banks[0])
            { std::printf("00a3ed30_TEST FAIL\n"); return 1; }
    }
    FreeNames(m.NameMap._M_start, 4);
    if (g_blocks != 0)
        { std::printf("00a3ed30_TEST FAIL\n"); return 1; }
    std::printf("00a3ed30_TEST PASS\n");
    return 0;
}