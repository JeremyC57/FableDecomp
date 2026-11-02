// Behaviour test for 0x00e398a0 NScript::CV_AssassinAttacksScript::~CV_AssassinAttacksScript() (implicit destructor): the
// RegionKillCounts block freed (when allocated), the RegionInfo[4..0] names, RegionOfMostRecentKill, then ~CScriptBase.
#include <cstdio>
#include <cstdlib>
#include <cstring>

static char g_log[64];
static int g_n = 0;
static int g_freed = 0;
static void Log(char c) { if (g_n < 63) g_log[g_n++] = c; g_log[g_n] = 0; }

class CCharString {
public:
    CCharString() : Id('?') { }
    ~CCharString() { Log(Id); }
    char Id;
};
struct LongVector {                                         // STLport vector<long>: trivial elements, block freed when set
    long* _M_start;
    long* _M_finish;
    long* _M_end_of_storage;
    LongVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~LongVector() { if (_M_start != 0) { std::free(_M_start); ++g_freed; Log('V'); } }
};
class CScriptBase { public: virtual ~CScriptBase() { Log('B'); } };
class CLevelScriptBase : public CScriptBase { public: void* Game; void* MasterScriptData; };
class CV_AssassinAttacksScript : public CLevelScriptBase {
public:
    class RegionData { public: CCharString name; long count; long max; bool assassinActive; };
    CV_AssassinAttacksScript()
    {
        RegionOfMostRecentKill.Id = 'r';
        for (int i = 0; i < 5; ++i) RegionInfo[i].name.Id = (char)('0' + i);
    }
    long        RegionIndex;
    long        AssassinObjectCount;
    bool        Activated;
    CCharString RegionOfMostRecentKill;
    RegionData  RegionInfo[5];
    LongVector  RegionKillCounts;
    bool        QuestComplete;
    bool        RewardsGiven;
    bool        GivenQuestCard;
};

int main()
{
    CV_AssassinAttacksScript* s = new CV_AssassinAttacksScript;
    s->RegionKillCounts._M_start = (long*)std::malloc(5 * sizeof(long));
    s->RegionKillCounts._M_finish = s->RegionKillCounts._M_end_of_storage = s->RegionKillCounts._M_start + 5;
    delete (CScriptBase*)s;
    if (std::strcmp(g_log, "V43210rB") != 0 || g_freed != 1) {
        std::printf("00e398a0_TEST FAIL (%s)\n", g_log);
        return 1;
    }
    g_n = 0; g_log[0] = 0;
    CScriptBase* t = new CV_AssassinAttacksScript;          // empty vector: nothing freed
    delete t;
    if (std::strcmp(g_log, "43210rB") != 0 || g_freed != 1) {
        std::printf("00e398a0_TEST FAIL (%s)\n", g_log);
        return 1;
    }
    std::printf("00e398a0_TEST PASS\n");
    return 0;
}