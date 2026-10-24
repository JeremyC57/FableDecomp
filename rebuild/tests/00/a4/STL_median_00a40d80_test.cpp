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
    CCharString& operator=(const CCharString& other)
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

namespace _STL {
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
    bool operator()(const _STL::pair<K, V>& a, const _STL::pair<K, V>& b) const { return a.first < b.first; }
    bool operator()(const K& a, const _STL::pair<K, V>& b) const { return a < b.first; }
    bool operator()(const _STL::pair<K, V>& a, const K& b) const { return a.first < b; }
};

typedef _STL::pair<CCharString, CCharString>      SNamePair;
typedef _STL::pair<CCharString, CSoundBankLH*>     SNameEntry;
typedef _STL::pair<SNamePair, CSoundBankLH*>       SNamePairEntry;
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
namespace _STL {
template <class _Tp, class _Compare>
const _Tp& __median(const _Tp& __a, const _Tp& __b, const _Tp& __c, _Compare __comp)
{
    if (__comp(__a, __b))
        if (__comp(__b, __c))
            return __b;
        else if (__comp(__a, __c))
            return __c;
        else
            return __a;
    else if (__comp(__a, __c))
        return __a;
    else if (__comp(__b, __c))
        return __c;
    else
        return __b;
}
}

int main()
{
    const char* names[6] = { "a", "x", "b", "x", "c", "x" };
    SNamePairEntry* p = MakePairs(names, 3, 3);
    CNamePairLess c;
    const SNamePairEntry *A0 = p, *B0 = p + 1, *C0 = p + 2;
    if (&_STL::__median(*A0, *B0, *C0, c) != B0 || &_STL::__median(*A0, *C0, *B0, c) != B0 || &_STL::__median(*B0, *A0, *C0, c) != B0
        || &_STL::__median(*B0, *C0, *A0, c) != B0 || &_STL::__median(*C0, *A0, *B0, c) != B0 || &_STL::__median(*C0, *B0, *A0, c) != B0
        || &_STL::__median(*A0, *A0, *C0, c) != A0)
        { std::printf("00a40d80_TEST FAIL\n"); return 1; }
    FreePairs(p, 3);
    if (g_blocks != 0)
        { std::printf("00a40d80_TEST FAIL\n"); return 1; }
    std::printf("00a40d80_TEST PASS\n");
    return 0;
}