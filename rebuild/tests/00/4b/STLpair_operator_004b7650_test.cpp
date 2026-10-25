// Behaviour test for 0x004B7650 pair<CCharString, CRuntimeQuestPersistence::CPersistInfo>::operator= (implicit).
#include <cstdio>
#include <cstring>

class CCharString {
public:
    const char* PData;
    static int s_copies;
    static int s_assigns;
    CCharString(const char* s) : PData(s) {}
    CCharString(const CCharString& o) : PData(o.PData) { ++s_copies; }
    CCharString& operator=(const CCharString& o) { PData = o.PData; ++s_assigns; return *this; }
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
    stlp::pair<CCharString, TInfo> a(CCharString("Q_A"), TInfo("QuestA", "PersistA"));
    stlp::pair<CCharString, TInfo> b(CCharString("Q_B"), TInfo("QuestB", "PersistB"));
    CCharString::s_assigns = 0;
    stlp::pair<CCharString, TInfo>* r = &(a = b);
    if (!(r == &a && CCharString::s_assigns == 3 && Same(a.first, "Q_B") && Same(a.second.QuestName, "QuestB") && Same(a.second.PersistScript, "PersistB"))) {
        std::printf("004b7650_TEST FAIL\n");
        return 1;
    }
    std::printf("004b7650_TEST PASS\n");
    return 0;
}