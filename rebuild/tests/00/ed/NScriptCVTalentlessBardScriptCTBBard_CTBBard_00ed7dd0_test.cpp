// Test for 0x00ED7DD0 CV_TalentlessBardScript::CTB_Bard::~CTB_Bard (implicit): lastRandomSpeech3, 2, 1, then ~CEntityScriptBase.
#include <cstdio>
static char g_log[16]; static int g_n = 0;
static void Log(char c) { if (g_n < 15) g_log[g_n++] = c; }
class CCharString { public: char Tag; ~CCharString() { Log(Tag); } };
class CEntityScriptBase { public: virtual ~CEntityScriptBase() { Log('E'); } void* Game; unsigned char Me[0xc]; };
namespace NScript {
class CV_TalentlessBardScript { public: class CTB_Bard; };
class CV_TalentlessBardScript::CTB_Bard : public CEntityScriptBase { public:
    CV_TalentlessBardScript* ParentClass; void* PMasterData; bool first_time_talked_to; bool given_gold;
    CCharString lastRandomSpeech1; CCharString lastRandomSpeech2; CCharString lastRandomSpeech3;
};
}
int main()
{
    NScript::CV_TalentlessBardScript::CTB_Bard* bard = new NScript::CV_TalentlessBardScript::CTB_Bard;
    bard->lastRandomSpeech1.Tag = '1'; bard->lastRandomSpeech2.Tag = '2'; bard->lastRandomSpeech3.Tag = '3';
    CEntityScriptBase* b = bard; delete b;
    g_log[g_n] = 0;
    const char* want = "321E";
    for (int i = 0; i < 5; ++i) if (g_log[i] != want[i]) { std::printf("00ed7dd0_TEST FAIL\n"); return 1; }
    std::printf("00ed7dd0_TEST PASS\n");
    return 0;
}