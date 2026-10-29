#include <cstdio>
#include <cstring>
enum EOpinionAttitudeType { ATT_LOVE = 0, ATT_LIKE = 1, ATT_NEUTRAL = 2 };
enum EAIActionReturn { AI_ACTION_RETURN_RUNNING = 0, AI_ACTION_RETURN_FAILED = 1, AI_ACTION_RETURN_FINISHED = 2 };
class CCharString {
public:
    CCharString(const char* text, long) { std::strcpy(m_text, text); }
    ~CCharString() { }
    const CCharString& operator+=(const CCharString& other) { std::strcat(m_text, other.m_text); return *this; }
    char m_text[128];
};
static CCharString g_OpinionAttitudeNames[3] = { CCharString("LOVE", -1), CCharString("LIKE", -1), CCharString("NEUTRAL", -1) };
class CAIStateGroup_OpinionReactionRoot { public:
    char m_said[128]; long m_layer; EAIActionReturn m_result; int m_contextCalls;
    long GetReactionContext() { ++m_contextCalls; return 0; }
    EAIActionReturn SayToThingReactingTo(const CCharString& text) { std::strcpy(m_said, text.m_text); m_layer = -1; return m_result; }
    EAIActionReturn WaitForTextToLoadThenSpeak(const CCharString& text, long layer) { std::strcpy(m_said, text.m_text); m_layer = layer; return m_result; }
    bool WifeFeelingUpdate(EOpinionAttitudeType attitude)
    {
        CCharString text("TEXT_AI_WIFE_OPINION_ACT_FEELING_", -1);
        GetReactionContext();
        text += g_OpinionAttitudeNames[attitude];
        return SayToThingReactingTo(text) == AI_ACTION_RETURN_FINISHED;
    }
};
int main()
{
    CAIStateGroup_OpinionReactionRoot g; g.m_contextCalls = 0; g.m_result = AI_ACTION_RETURN_RUNNING;
    if (g.WifeFeelingUpdate(ATT_LIKE)) { std::printf("00982eb0_TEST FAIL\n"); return 1; }
    if (std::strcmp(g.m_said, "TEXT_AI_WIFE_OPINION_ACT_FEELING_LIKE") != 0 || g.m_contextCalls != 1) { std::printf("00982eb0_TEST FAIL\n"); return 1; }
    g.m_result = AI_ACTION_RETURN_FINISHED;
    if (!g.WifeFeelingUpdate(ATT_NEUTRAL) || std::strcmp(g.m_said, "TEXT_AI_WIFE_OPINION_ACT_FEELING_NEUTRAL") != 0) { std::printf("00982eb0_TEST FAIL\n"); return 1; }
    g.m_result = AI_ACTION_RETURN_FAILED;
    if (g.WifeFeelingUpdate(ATT_LOVE)) { std::printf("00982eb0_TEST FAIL\n"); return 1; }
    if (g.m_layer != -1) { std::printf("00982eb0_TEST FAIL\n"); return 1; }
    std::printf("00982eb0_TEST PASS\n");
    return 0;
}