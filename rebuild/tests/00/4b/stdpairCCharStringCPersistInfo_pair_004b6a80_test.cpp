// Behaviour test for 0x004B6A80 (implicit ~pair<CCharString, CRuntimeQuestPersistence::CPersistInfo>): destroys
// second.PersistScript, second.QuestName, then first, each once.
#include <cstdio>
#include <new>

static int g_log[16];
static int g_count = 0;
static void Log(int id) { if (g_count < 16) g_log[g_count++] = id; }

class CCharString {                                      // model: records its id when destroyed
public:
    CCharString() : Id(0) {}
    ~CCharString() { Log(Id); }
    int Id;
};

namespace NQuestManager { class CRuntimeQuestPersistence { public:
    class CPersistInfo { public: CCharString QuestName; CCharString PersistScript; };
}; }
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }
typedef std::pair<CCharString, NQuestManager::CRuntimeQuestPersistence::CPersistInfo> TEntry;

int main()
{
    unsigned char storage[sizeof(TEntry) + 8];
    TEntry* e = new (storage) TEntry;
    e->first.Id = 1; e->second.QuestName.Id = 2; e->second.PersistScript.Id = 3;
    e->TEntry::~TEntry();
    if (g_count != 3 || g_log[0] != 3 || g_log[1] != 2 || g_log[2] != 1) { std::printf("004b6a80_TEST FAIL\n"); return 1; }
    std::printf("004b6a80_TEST PASS\n");
    return 0;
}