// Behaviour test for 0x004B7490 pair<CCharString, CRuntimeQuestPersistence::CPersistInfo> copy constructor.
#include <cstdio>
#include <cstring>

class CCharString {
public:
    const char* PData;
    static int s_copies;
    static int s_assigns;
    CCharString(const char* s) : PData(s) {}
    CCharString(const CCharString& o) : PData(o.PData) { ++s_copies; }
    const CCharString& operator=(const CCharString& o) { PData = o.PData; ++s_assigns; return *this; }
};
int CCharString::s_copies = 0;
int CCharString::s_assigns = 0;
static bool Same(const CCharString& s, const char* t) { return std::strcmp(s.PData, t) == 0; }
namespace NQuestManager {
class CRuntimeQuestPersistence {
public:
    class CPersistInfo {
    public:
        CCharString QuestName;
        CCharString PersistScript;
        CPersistInfo(const char* q, const char* s) : QuestName(q), PersistScript(s) {}
    };
};
}
typedef NQuestManager::CRuntimeQuestPersistence::CPersistInfo TInfo;
namespace stlp {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const _T1& a, const _T2& b) : first(a), second(b) {}
    pair(const pair& p) : first(p.first), second(p.second) {}
};
}

int main()
{
    stlp::pair<CCharString, TInfo> a(CCharString("Q_MAZE"), TInfo("QuestMaze", "MazePersist"));
    CCharString::s_copies = 0;
    stlp::pair<CCharString, TInfo> b(a);
    if (!(CCharString::s_copies == 3 && Same(b.first, "Q_MAZE") && Same(b.second.QuestName, "QuestMaze") && Same(b.second.PersistScript, "MazePersist"))) {
        std::printf("004b7490_TEST FAIL\n");
        return 1;
    }
    std::printf("004b7490_TEST PASS\n");
    return 0;
}