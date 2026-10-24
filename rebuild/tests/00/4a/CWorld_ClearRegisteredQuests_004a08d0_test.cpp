#include <cstdio>
#include <new>

// Behaviour model of CWorld::ClearRegisteredQuests (0x004A08D0): clear() = erase(begin(), end()) on three lists.
static int g_strings = 0, g_quests = 0;

struct CCharString {
    const char* p;
    CCharString(const char* s) : p(s) { ++g_strings; }
    CCharString(const CCharString& o) : p(o.p) { ++g_strings; }
    ~CCharString() { --g_strings; }
    CCharString& operator=(const CCharString& o) { p = o.p; return *this; }
};

struct CTestQuest {
    CCharString QuestName, HSPName;
    long        ColourIndex;
    CCharString DisplayName, IniFile, OutiFile, QuestCardDefName;
    CTestQuest(const char* n) : QuestName(n), HSPName(""), ColourIndex(3), DisplayName(""), IniFile(""), OutiFile(""), QuestCardDefName("") { ++g_quests; }
    ~CTestQuest() { --g_quests; }
};

template <class T>
struct Vec {
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    T* erase(T* first, T* last)
    {
        T* d = first;
        for (T* s = last; s != _M_finish; ++s, ++d) *d = *s;
        for (T* q = d; q != _M_finish; ++q) q->~T();
        _M_finish = d;
        return first;
    }
    void clear() { erase(_M_start, _M_finish); }
};

struct CWorld {
    Vec<CCharString> InitialActiveQuests;
    Vec<CCharString> RegisteredQuests;
    Vec<CTestQuest>  ActiveTestQuests;
    void ClearRegisteredQuests()
    {
        RegisteredQuests.clear();
        InitialActiveQuests.clear();
        ActiveTestQuests.clear();
    }
};

static unsigned char g_a[sizeof(CCharString) * 4], g_b[sizeof(CCharString) * 4], g_c[sizeof(CTestQuest) * 4];

int main()
{
    CWorld w;
    CCharString* a = (CCharString*)g_a; CCharString* b = (CCharString*)g_b; CTestQuest* c = (CTestQuest*)g_c;
    new (&a[0]) CCharString("Q1"); new (&a[1]) CCharString("Q2");
    new (&b[0]) CCharString("R1"); new (&b[1]) CCharString("R2"); new (&b[2]) CCharString("R3");
    new (&c[0]) CTestQuest("T1");
    w.InitialActiveQuests._M_start = a; w.InitialActiveQuests._M_finish = a + 2; w.InitialActiveQuests._M_end_of_storage = a + 4;
    w.RegisteredQuests._M_start = b; w.RegisteredQuests._M_finish = b + 3; w.RegisteredQuests._M_end_of_storage = b + 4;
    w.ActiveTestQuests._M_start = c; w.ActiveTestQuests._M_finish = c + 1; w.ActiveTestQuests._M_end_of_storage = c + 4;
    if (g_strings != 5 + 6 || g_quests != 1) { std::printf("004a08d0_TEST FAIL\n"); return 1; }
    w.ClearRegisteredQuests();
    if (w.InitialActiveQuests._M_finish != a || w.RegisteredQuests._M_finish != b || w.ActiveTestQuests._M_finish != c) { std::printf("004a08d0_TEST FAIL\n"); return 1; }
    if (w.RegisteredQuests._M_end_of_storage != b + 4) { std::printf("004a08d0_TEST FAIL\n"); return 1; }   // capacity kept
    if (g_strings != 0 || g_quests != 0) { std::printf("004a08d0_TEST FAIL\n"); return 1; }
    w.ClearRegisteredQuests();                                                                          // already empty: no-op
    if (g_strings != 0 || w.ActiveTestQuests._M_finish != c) { std::printf("004a08d0_TEST FAIL\n"); return 1; }
    std::printf("004a08d0_TEST PASS\n");
    return 0;
}