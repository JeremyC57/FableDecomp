// Behaviour test for 0x00AF3700 std::__insertion_sort over pair<CCharString, {long, long, CCharString}> records.
#include <cstdio>
#include <cstring>

static int g_stringCopies = 0;
class CCharString { public:
    const char* m_pData;
    CCharString(const char* s) : m_pData(s) { }
    CCharString(const CCharString& other) : m_pData(other.m_pData) { ++g_stringCopies; }
    bool operator<(const CCharString& o) const { return std::strcmp(m_pData, o.m_pData) < 0; }
};
struct SNamedPayload { long m_value0; long m_value1; CCharString m_name; };
struct SNamedPairRecord {
    CCharString first; SNamedPayload second;
    SNamedPairRecord(const char* k, long a, long b, const char* n) : first(k), second(make(a, b, n)) { }
    SNamedPairRecord(const SNamedPairRecord& o) : first(o.first), second(o.second) { }
    static SNamedPayload make(long a, long b, const char* n) { SNamedPayload p = { a, b, CCharString(n) }; return p; }
};
struct CKeyPairCompareLess {
    void* m_context;
    bool operator()(const SNamedPairRecord& a, const SNamedPairRecord& b) const { return a.first < b.first; }
};

static int g_calls = 0;
static void __linear_insert(SNamedPairRecord* first, SNamedPairRecord* last, SNamedPairRecord value, CKeyPairCompareLess comp)
{
    ++g_calls;
    if (comp(value, *first)) {
        for (SNamedPairRecord* p = last; p != first; --p) *p = *(p - 1);
        *first = value;
    } else {
        SNamedPairRecord* next = last - 1;
        while (comp(value, *next)) { *last = *next; last = next; --next; }
        *last = value;
    }
}
static void __insertion_sort(SNamedPairRecord* first, SNamedPairRecord* last, CKeyPairCompareLess comp)
{
    if (first == last)
        return;
    for (SNamedPairRecord* i = first + 1; i != last; ++i)
        __linear_insert(first, i, *i, comp);
}

int main()
{
    SNamedPairRecord a[4] = { SNamedPairRecord("m", 3, 30, "three"), SNamedPairRecord("b", 1, 10, "one"),
                              SNamedPairRecord("z", 4, 40, "four"), SNamedPairRecord("c", 2, 20, "two") };
    CKeyPairCompareLess comp = { 0 };
    g_stringCopies = 0;
    __insertion_sort(a, a + 4, comp);
    const char* keys[4] = { "b", "c", "m", "z" };
    const char* names[4] = { "one", "two", "three", "four" };
    for (int k = 0; k < 4; ++k)
        if (std::strcmp(a[k].first.m_pData, keys[k]) != 0 || std::strcmp(a[k].second.m_name.m_pData, names[k]) != 0
            || a[k].second.m_value0 != k + 1 || a[k].second.m_value1 != 10 * (k + 1)) {
            std::printf("00af3700_TEST FAIL\n"); return 1;
        }
    if (g_calls != 3 || g_stringCopies != 6) { std::printf("00af3700_TEST FAIL\n"); return 1; }
    std::printf("00af3700_TEST PASS\n");
    return 0;
}