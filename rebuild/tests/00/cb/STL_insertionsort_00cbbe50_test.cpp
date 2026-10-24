// Behaviour test for 0x00CBBE50 _STL::__insertion_sort over {unsigned __int64 key, long, long, CCharString} records.
#include <cstdio>
#include <cstring>

static int g_stringCopies = 0;
struct CCharString {
    const char* m_pData;
    CCharString(const char* s) : m_pData(s) { }
    CCharString(const CCharString& other) : m_pData(other.m_pData) { ++g_stringCopies; }
};

typedef unsigned long long U64;
struct SKeyedNameRecord {
    U64 m_key; long m_value0; long m_value1; CCharString m_name;
    SKeyedNameRecord(U64 k, long a, long b, const char* n) : m_key(k), m_value0(a), m_value1(b), m_name(n) { }
    SKeyedNameRecord(const SKeyedNameRecord& o) : m_key(o.m_key), m_value0(o.m_value0), m_value1(o.m_value1), m_name(o.m_name) { }
};
struct CKeyPairCompareLess {
    void* m_context;
    bool operator()(const SKeyedNameRecord& a, const SKeyedNameRecord& b) const { return a.m_key < b.m_key; }
};

static int g_calls = 0;
static void __linear_insert(SKeyedNameRecord* first, SKeyedNameRecord* last, SKeyedNameRecord value, CKeyPairCompareLess comp)
{
    ++g_calls;
    if (comp(value, *first)) {
        for (SKeyedNameRecord* p = last; p != first; --p) *p = *(p - 1);
        *first = value;
    } else {
        SKeyedNameRecord* next = last - 1;
        while (comp(value, *next)) { *last = *next; last = next; --next; }
        *last = value;
    }
}
static void __insertion_sort(SKeyedNameRecord* first, SKeyedNameRecord* last, CKeyPairCompareLess comp)
{
    if (first == last)
        return;
    for (SKeyedNameRecord* i = first + 1; i != last; ++i)
        __linear_insert(first, i, *i, comp);
}

int main()
{
    SKeyedNameRecord a[4] = { SKeyedNameRecord(30, 3, 33, "c"), SKeyedNameRecord(0x200000000ull, 4, 44, "d"),
                              SKeyedNameRecord(10, 1, 11, "a"), SKeyedNameRecord(20, 2, 22, "b") };
    CKeyPairCompareLess comp = { 0 };
    g_stringCopies = 0;
    __insertion_sort(a, a + 4, comp);
    const char* names[4] = { "a", "b", "c", "d" };
    for (int k = 0; k < 4; ++k)
        if (std::strcmp(a[k].m_name.m_pData, names[k]) != 0 || a[k].m_value0 != k + 1 || a[k].m_value1 != 11 * (k + 1)) {
            std::printf("00cbbe50_TEST FAIL\n"); return 1;
        }
    if (g_calls != 3 || g_stringCopies != 3) { std::printf("00cbbe50_TEST FAIL\n"); return 1; }   // one by-value copy per insert
    std::printf("00cbbe50_TEST PASS\n");
    return 0;
}