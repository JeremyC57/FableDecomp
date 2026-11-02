// Behaviour test for 0x00E37680 implicit ~CV_ArcheryCompetitionScript: the ten RandomSpeech strings from the top, then
// RandomSpeechTimer, then GameTimeLeft (each deregistering its timer), then ~CScriptBase.
#include <cstdio>
#include <cstring>
static char g_log[64];
static void Log(const char* s) { std::strcat(g_log, s); }
static int g_deregistered[4];
static int g_count = 0;
class CTimer { public: CTimer(int i) : TimerIndex(i) { } ~CTimer() { g_deregistered[g_count++] = TimerIndex; } int TimerIndex; };
class CCharString { public: CCharString() : C('?') { } ~CCharString() { char b[2] = { C, 0 }; Log(b); } char C; };
class CScriptBase { public: virtual ~CScriptBase() { Log("B"); } };
class CLevelScriptBase : public CScriptBase { };
class CV_ArcheryCompetitionScript : public CLevelScriptBase {
public:
    CV_ArcheryCompetitionScript() : GameTimeLeft(7), RandomSpeechTimer(9) { for (int i = 0; i < 10; ++i) RandomSpeech[i].C = (char)('0' + i); }
    long Score;
    CTimer GameTimeLeft;
    CTimer RandomSpeechTimer;
    CCharString RandomSpeech[10];
    int ContestState;
};
int main()
{
    g_log[0] = 0;
    CScriptBase* p = new CV_ArcheryCompetitionScript;
    delete p;
    bool ok = std::strcmp(g_log, "9876543210B") == 0 && g_count == 2 && g_deregistered[0] == 9 && g_deregistered[1] == 7;
    std::printf(ok ? "00e37680_TEST PASS\n" : "00e37680_TEST FAIL\n");
    return ok ? 0 : 1;
}