// Behaviour test for 0x00e542b0 NScript::CV_BeardyBaldyScript::~CV_BeardyBaldyScript() (implicit destructor): RandomSpeech[9..0],
// the RandomSpeechTimer deregistration, RequiredTash, RequiredBeard, RequiredHairdo, then ~CScriptBase.
#include <cstdio>
#include <cstring>

static char g_log[64];
static int g_n = 0;
static long g_deregistered = -1;
static void Log(char c) { if (g_n < 63) g_log[g_n++] = c; g_log[g_n] = 0; }

class CCharString {
public:
    CCharString() : Id('?') { }
    explicit CCharString(char id) : Id(id) { }
    ~CCharString() { Log(Id); }
    char Id;
};
class CTimer {
public:
    CTimer() : TimerIndex(7) { }
    ~CTimer() { Log('T'); g_deregistered = TimerIndex; }
    long TimerIndex;
};
class CScriptBase { public: virtual ~CScriptBase() { Log('B'); } };
class CLevelScriptBase : public CScriptBase { public: void* Game; void* MasterScriptData; };
class CV_BeardyBaldyScript : public CLevelScriptBase {
public:
    CV_BeardyBaldyScript() : RequiredHairdo('h'), RequiredBeard('b'), RequiredTash('t')
    { for (int i = 0; i < 10; ++i) RandomSpeech[i].Id = (char)('0' + i); }
    bool        QuestFlags[3];
    long        QuestPhase;
    bool        PhaseFlags[5];
    CCharString RequiredHairdo;
    CCharString RequiredBeard;
    CCharString RequiredTash;
    bool        HairFlags[7];
    long        IncorrectHairComboCount;
    bool        AttackedByHero;
    CTimer      RandomSpeechTimer;
    CCharString RandomSpeech[10];
    long        BeardyBaldyPhase;
};

int main()
{
    CScriptBase* s = new CV_BeardyBaldyScript;
    delete s;
    if (std::strcmp(g_log, "9876543210TtbhB") != 0 || g_deregistered != 7) {
        std::printf("00e542b0_TEST FAIL (%s)\n", g_log);
        return 1;
    }
    std::printf("00e542b0_TEST PASS\n");
    return 0;
}