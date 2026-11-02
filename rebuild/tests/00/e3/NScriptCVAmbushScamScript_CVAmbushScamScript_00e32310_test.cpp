// Behaviour test for 0x00e32310 NScript::CV_AmbushScamScript::~CV_AmbushScamScript() (implicit destructor): RandomSpeech[4..0],
// then the RandomSpeechTimer deregistered with its own index, then ~CScriptBase.
#include <cstdio>
#include <cstring>

static char g_log[64];
static int g_n = 0;
static long g_deregistered = -1;
static void Log(char c) { if (g_n < 63) g_log[g_n++] = c; g_log[g_n] = 0; }

class CCharString {
public:
    CCharString() : Id('?') { }
    ~CCharString() { Log(Id); }
    char Id;
};
class CTimer {
public:
    CTimer() : TimerIndex(42) { }
    ~CTimer() { Log('T'); g_deregistered = TimerIndex; }     // pDSTGame->DeregisterTimer(TimerIndex)
    long TimerIndex;
};
class CScriptBase { public: virtual ~CScriptBase() { Log('B'); } };
class CLevelScriptBase : public CScriptBase { public: void* Game; void* MasterScriptData; };
class CV_AmbushScamScript : public CLevelScriptBase {
public:
    CV_AmbushScamScript() { for (int i = 0; i < 5; ++i) RandomSpeech[i].Id = (char)('0' + i); }
    bool        Flags[0x2c];
    long        HitCount;
    long        BuddyCounter;
    CTimer      RandomSpeechTimer;
    CCharString RandomSpeech[5];
};

int main()
{
    CScriptBase* s = new CV_AmbushScamScript;
    delete s;
    if (std::strcmp(g_log, "43210TB") != 0 || g_deregistered != 42) {
        std::printf("00e32310_TEST FAIL (%s)\n", g_log);
        return 1;
    }
    std::printf("00e32310_TEST PASS\n");
    return 0;
}