// Behaviour test for 0x00CB6EA0 std::__insertion_sort over the script registry's pair<CCharString, CScriptInfo>.
#include <cstdio>
#include <cstring>

static int g_stringCopies = 0;
struct CCharString {
    const char* m_pData;
    CCharString(const char* s) : m_pData(s) { }
    CCharString(const CCharString& other) : m_pData(other.m_pData) { ++g_stringCopies; }
    bool operator<(const CCharString& o) const { return std::strcmp(m_pData, o.m_pData) < 0; }
};
struct CScriptBase; struct CScriptDataBase; struct CGameScriptInterfaceBase;
struct CScriptInfo {
    CScriptBase*     (*AllocFunc)(CScriptDataBase*, CGameScriptInterfaceBase*);
    CScriptDataBase* (*MasterScriptDataAlloc)();
    CCharString      Name;
    long             ID;
    bool             MasterScript;
};
struct PairCharStringScriptInfo {
    CCharString first; CScriptInfo second;
    PairCharStringScriptInfo(const char* k, long id, bool master) : first(k), second(make(k, id, master)) { }
    PairCharStringScriptInfo(const PairCharStringScriptInfo& o) : first(o.first), second(o.second) { }
    static CScriptInfo make(const char* n, long id, bool m) { CScriptInfo i = { 0, 0, CCharString(n), id, m }; return i; }
};
struct CCompareScriptName {
    void* m_context;
    bool operator()(const PairCharStringScriptInfo& a, const PairCharStringScriptInfo& b) const { return a.first < b.first; }
};

static int g_calls = 0;
static void __linear_insert(PairCharStringScriptInfo* first, PairCharStringScriptInfo* last, PairCharStringScriptInfo value, CCompareScriptName comp)
{
    ++g_calls;
    if (comp(value, *first)) {
        for (PairCharStringScriptInfo* p = last; p != first; --p) *p = *(p - 1);
        *first = value;
    } else {
        PairCharStringScriptInfo* next = last - 1;
        while (comp(value, *next)) { *last = *next; last = next; --next; }
        *last = value;
    }
}
static void __insertion_sort(PairCharStringScriptInfo* first, PairCharStringScriptInfo* last, CCompareScriptName comp)
{
    if (first == last)
        return;
    for (PairCharStringScriptInfo* i = first + 1; i != last; ++i)
        __linear_insert(first, i, *i, comp);
}

int main()
{
    PairCharStringScriptInfo a[4] = { PairCharStringScriptInfo("Q_Oakvale", 3, false), PairCharStringScriptInfo("Q_Bowerstone", 1, true),
                                      PairCharStringScriptInfo("Q_Witchwood", 4, false), PairCharStringScriptInfo("Q_Greatwood", 2, true) };
    CCompareScriptName comp = { 0 };
    g_stringCopies = 0;
    __insertion_sort(a, a + 4, comp);
    for (int k = 0; k < 4; ++k)
        if (a[k].second.ID != k + 1 || std::strcmp(a[k].first.m_pData, a[k].second.Name.m_pData) != 0 || a[k].second.MasterScript != (k < 2)) {
            std::printf("00cb6ea0_TEST FAIL\n"); return 1;
        }
    if (g_calls != 3 || g_stringCopies != 6) { std::printf("00cb6ea0_TEST FAIL\n"); return 1; }
    std::printf("00cb6ea0_TEST PASS\n");
    return 0;
}