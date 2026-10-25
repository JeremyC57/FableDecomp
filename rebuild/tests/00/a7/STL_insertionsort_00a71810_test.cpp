// Behaviour test for 0x00A71810 std::__insertion_sort over pair<CCharString, 52-byte POD> records.
#include <cstdio>
#include <cstring>

static int g_stringCopies = 0;
struct CCharString {
    const char* m_pData;
    CCharString(const char* s) : m_pData(s) { }
    CCharString(const CCharString& other) : m_pData(other.m_pData) { ++g_stringCopies; }
    bool operator<(const CCharString& o) const { return std::strcmp(m_pData, o.m_pData) < 0; }
};
struct SRecordData { long m_words[13]; };
struct SNamedRecord {
    CCharString first; SRecordData second;
    SNamedRecord(const char* n, long w) : first(n) { for (int k = 0; k < 13; ++k) second.m_words[k] = w + k; }
    SNamedRecord(const SNamedRecord& o) : first(o.first), second(o.second) { }
};
struct SNameLess {
    void* m_context;
    bool operator()(const SNamedRecord& a, const SNamedRecord& b) const { return a.first < b.first; }
};

static int g_calls = 0;
static void __linear_insert(SNamedRecord* first, SNamedRecord* last, SNamedRecord value, SNameLess comp)
{
    ++g_calls;
    if (comp(value, *first)) {
        for (SNamedRecord* p = last; p != first; --p) *p = *(p - 1);
        *first = value;
    } else {
        SNamedRecord* next = last - 1;
        while (comp(value, *next)) { *last = *next; last = next; --next; }
        *last = value;
    }
}
static void __insertion_sort(SNamedRecord* first, SNamedRecord* last, SNameLess comp)
{
    if (first == last)
        return;
    for (SNamedRecord* i = first + 1; i != last; ++i)
        __linear_insert(first, i, *i, comp);
}

int main()
{
    SNamedRecord a[4] = { SNamedRecord("dummy_head", 100), SNamedRecord("bone_root", 200),
                          SNamedRecord("muzzle", 300), SNamedRecord("eye_l", 400) };
    SNameLess comp = { 0 };
    g_stringCopies = 0;
    __insertion_sort(a, a + 4, comp);
    const char* names[4] = { "bone_root", "dummy_head", "eye_l", "muzzle" };
    long bases[4] = { 200, 100, 400, 300 };
    for (int k = 0; k < 4; ++k)
        if (std::strcmp(a[k].first.m_pData, names[k]) != 0 || a[k].second.m_words[0] != bases[k] || a[k].second.m_words[12] != bases[k] + 12) {
            std::printf("00a71810_TEST FAIL\n"); return 1;
        }
    if (g_calls != 3 || g_stringCopies != 3) { std::printf("00a71810_TEST FAIL\n"); return 1; }
    std::printf("00a71810_TEST PASS\n");
    return 0;
}