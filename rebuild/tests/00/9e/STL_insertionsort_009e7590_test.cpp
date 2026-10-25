// Behaviour test for 0x009E7590 std::__insertion_sort over pair<CCharString, CSymbolMap::CSymbolInfo> records.
#include <cstdio>
#include <cstring>

static int g_stringCopies = 0;
struct CCharString {
    const char* m_pData;
    CCharString(const char* s) : m_pData(s) { }
    CCharString(const CCharString& other) : m_pData(other.m_pData) { ++g_stringCopies; }
    bool operator<(const CCharString& o) const { return std::strcmp(m_pData, o.m_pData) < 0; }
};
struct CSymbolInfo { CCharString GroupName; CCharString EntryName; long Val; };
struct SSymbolRecord {
    CCharString first; CSymbolInfo second;
    SSymbolRecord(const char* n, const char* g, const char* e, long v) : first(n), second(make(g, e, v)) { }
    SSymbolRecord(const SSymbolRecord& o) : first(o.first), second(o.second) { }
    static CSymbolInfo make(const char* g, const char* e, long v) { CSymbolInfo i = { CCharString(g), CCharString(e), v }; return i; }
};
struct CCompareString {
    void* m_context;
    bool operator()(const SSymbolRecord& a, const SSymbolRecord& b) const { return a.first < b.first; }
};

static int g_calls = 0;
static void __linear_insert(SSymbolRecord* first, SSymbolRecord* last, SSymbolRecord value, CCompareString comp)
{
    ++g_calls;
    if (comp(value, *first)) {
        for (SSymbolRecord* p = last; p != first; --p) *p = *(p - 1);
        *first = value;
    } else {
        SSymbolRecord* next = last - 1;
        while (comp(value, *next)) { *last = *next; last = next; --next; }
        *last = value;
    }
}
static void __insertion_sort(SSymbolRecord* first, SSymbolRecord* last, CCompareString comp)
{
    if (first == last)
        return;
    for (SSymbolRecord* i = first + 1; i != last; ++i)
        __linear_insert(first, i, *i, comp);
}

int main()
{
    SSymbolRecord a[3] = { SSymbolRecord("QUEST_B", "quests", "b", 2), SSymbolRecord("QUEST_C", "quests", "c", 3),
                           SSymbolRecord("QUEST_A", "quests", "a", 1) };
    CCompareString comp = { 0 };
    g_stringCopies = 0;
    __insertion_sort(a, a + 3, comp);
    const char* names[3] = { "QUEST_A", "QUEST_B", "QUEST_C" };
    const char* entries[3] = { "a", "b", "c" };
    for (int k = 0; k < 3; ++k)
        if (std::strcmp(a[k].first.m_pData, names[k]) != 0 || std::strcmp(a[k].second.EntryName.m_pData, entries[k]) != 0 || a[k].second.Val != k + 1) {
            std::printf("009e7590_TEST FAIL\n"); return 1;
        }
    if (g_calls != 2 || g_stringCopies != 6) { std::printf("009e7590_TEST FAIL\n"); return 1; }   // three strings per by-value copy
    std::printf("009e7590_TEST PASS\n");
    return 0;
}