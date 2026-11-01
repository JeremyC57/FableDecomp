// Behaviour test for 0x0071A570 NTCScriptedControl::CActionTalkToThing::~CActionTalkToThing (implicit): QuestionButton3,
// QuestionButton2, QuestionButton1 are destroyed in that order, then ~CActionTalkBase.
#include <cstdio>
static int g_order[8];
static int g_count = 0;
class CCharString {
public:
    int id;
    CCharString() : id(0) {}
    ~CCharString() { g_order[g_count++] = id; }
};
namespace NTCScriptedControl {
class CActionTalkBase {
public:
    CActionTalkBase() {}
    virtual ~CActionTalkBase() { g_order[g_count++] = 9; }
    unsigned char _members[0x90 - sizeof(void*)];
};
class CActionTalkToThing : public CActionTalkBase {
public:
    CActionTalkToThing() : AddedDuringCutscene(false), AskingHeroQuestion(false), DisplayOverScreenFade(false)
    {
        QuestionButton1.id = 1; QuestionButton2.id = 2; QuestionButton3.id = 3;
    }
    bool        AddedDuringCutscene;
    bool        AskingHeroQuestion;
    bool        DisplayOverScreenFade;
    CCharString QuestionButton1;
    CCharString QuestionButton2;
    CCharString QuestionButton3;
};
}
int main()
{
    NTCScriptedControl::CActionTalkBase* p = new NTCScriptedControl::CActionTalkToThing;
    delete p;
    if (g_count != 4 || g_order[0] != 3 || g_order[1] != 2 || g_order[2] != 1 || g_order[3] != 9) {
        std::printf("0071a570_TEST FAIL\n"); return 1;
    }
    std::printf("0071a570_TEST PASS\n");
    return 0;
}