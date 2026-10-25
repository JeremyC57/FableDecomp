// Behaviour test for 0x004B7800 _STL::swap<pair<CCharString,NQuestManager::CRuntimeQuestPersistence::CPersistInfo>>.
#include <cstdio>
#include <cstring>
#define CHECK(c) do { if (!(c)) { std::printf("004b7800_TEST FAIL\n"); return 1; } } while (0)

static int g_copies = 0, g_assigns = 0, g_dtors = 0;
class CCharString {                                     // test model: a short owned text
public:
    char Text[16];
    CCharString() { Text[0] = 0; }
    CCharString(const char* s) { std::strncpy(Text, s, 15); Text[15] = 0; }
    CCharString(const CCharString& o) { std::memcpy(Text, o.Text, 16); ++g_copies; }
    ~CCharString() { ++g_dtors; }
    CCharString& operator=(const CCharString& o) { std::memcpy(Text, o.Text, 16); ++g_assigns; return *this; }
    bool Is(const char* s) const { return std::strcmp(Text, s) == 0; }
};

namespace NQuestManager {
class CRuntimeQuestPersistence {
public:
    class CPersistInfo {
    public:
        CCharString QuestName;
        CCharString PersistScript;
        CPersistInfo(const char* q, const char* p) : QuestName(q), PersistScript(p) { }
    };
};
}

template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) {}
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) {}
};

template <class _Tp> inline void swap_model(_Tp& __a, _Tp& __b)    // STLport swap
{
    _Tp __tmp = __a;
    __a = __b;
    __b = __tmp;
}

int main()
{
    typedef pair<CCharString, NQuestManager::CRuntimeQuestPersistence::CPersistInfo> TPair;
    TPair a(CCharString("Q1"), NQuestManager::CRuntimeQuestPersistence::CPersistInfo("QuestOne", "PersistOne"));
    TPair b(CCharString("Q2"), NQuestManager::CRuntimeQuestPersistence::CPersistInfo("QuestTwo", "PersistTwo"));
    g_copies = g_assigns = 0;
    swap_model(a, b);
    CHECK(a.first.Is("Q2") && a.second.QuestName.Is("QuestTwo") && a.second.PersistScript.Is("PersistTwo"));
    CHECK(b.first.Is("Q1") && b.second.QuestName.Is("QuestOne") && b.second.PersistScript.Is("PersistOne"));
    CHECK(g_copies == 3 && g_assigns == 6);
    std::printf("004b7800_TEST PASS\n");
    return 0;
}