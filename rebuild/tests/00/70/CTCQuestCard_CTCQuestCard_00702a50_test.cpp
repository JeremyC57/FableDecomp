// Behaviour test for 0x00702a50 CTCQuestCard::~CTCQuestCard() (implicit destructor): the five CCharString members in reverse
// order (ScriptQuestName .. QuestName), then the PDef def-pointer release (count down, Delete at zero, cleared), then ~CTCBase.
#include <cstdio>
#include <cstring>

static char g_log[64];
static int g_n = 0;
static void Log(char c) { if (g_n < 63) g_log[g_n++] = c; g_log[g_n] = 0; }

class CCharString {
public:
    explicit CCharString(char id) : Id(id) { }
    ~CCharString() { Log(Id); }
    char Id;
};
class CDefPointeeBase {
public:
    CDefPointeeBase() : IVRefCount(1) { }
    virtual ~CDefPointeeBase() { }
    virtual void Delete() const { Log('X'); delete this; }
    mutable unsigned int IVRefCount;
};
class CQuestCardDef : public CDefPointeeBase { };
template <class T> class CDefPointer {
public:
    explicit CDefPointer(const T* object) : Object((CDefPointeeBase*)object) { if (Object) ++Object->IVRefCount; }
    ~CDefPointer()
    {
        if (Object) {
            CDefPointeeBase* object = Object;
            if (--object->IVRefCount == 0)
                object->Delete();
            Object = 0;
            Log('P');
        }
    }
    CDefPointeeBase* Object;
};
class CTCBase { public: virtual ~CTCBase() { Log('B'); } };
class CTCQuestCard : public CTCBase {
public:
    explicit CTCQuestCard(const CQuestCardDef* def)
        : PDef(def), Finished(false), Replayable(false), FeatIndex(-1), QuestName('1'), ObjectiveName('2'), RegionName('3'),
          TeleporterRegionName('4'), ScriptQuestName('5'), GoldReward(0), RenownReward(0) { }
    CDefPointer<const CQuestCardDef> PDef;
    bool        Finished;
    bool        Replayable;
    long        FeatIndex;
    CCharString QuestName;
    CCharString ObjectiveName;
    CCharString RegionName;
    CCharString TeleporterRegionName;
    CCharString ScriptQuestName;
    long        GoldReward;
    long        RenownReward;
};

int main()
{
    CQuestCardDef* shared = new CQuestCardDef;             // count 1 (the owner)
    CTCBase* a = new CTCQuestCard(shared);                 // count 2
    delete a;                                              // count 1: kept
    if (std::strcmp(g_log, "54321PB") != 0 || shared->IVRefCount != 1) {
        std::printf("00702a50_TEST FAIL (%s)\n", g_log);
        return 1;
    }
    g_n = 0; g_log[0] = 0;
    shared->IVRefCount = 0;                                // the card holds the only reference
    CTCBase* b = new CTCQuestCard(shared);
    delete b;                                              // count 0: Delete()
    if (std::strcmp(g_log, "54321XPB") != 0) {
        std::printf("00702a50_TEST FAIL (%s)\n", g_log);
        return 1;
    }
    g_n = 0; g_log[0] = 0;
    CTCBase* c = new CTCQuestCard(0);                      // no def: nothing released
    delete c;
    if (std::strcmp(g_log, "54321B") != 0) {
        std::printf("00702a50_TEST FAIL (%s)\n", g_log);
        return 1;
    }
    std::printf("00702a50_TEST PASS\n");
    return 0;
}