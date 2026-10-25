// Behaviour test for 0x004CD670 CConversationAttitude::Init(): the three attitude-name tables are filled exactly once
// (18 EAttitude talk animations, 12 EConversationAttitude talk animations, 12 conversation anim groups), and a second
// call leaves them alone.
#include <cstdio>
#include <cstring>

struct CCharString {
    const char* text;
    int assigns;
    CCharString() : text(""), assigns(0) {}
    CCharString& operator=(const char* t) { text = t; ++assigns; return *this; }
};
struct CCharStringVector {
    CCharString* _M_start;
    CCharString& operator[](unsigned int n) { return *(_M_start + n); }
};
static CCharString g_store1[18], g_store2[12], g_store3[12];
static bool g_ConversationAttitudeInitialised = false;
static CCharStringVector g_AttitudeAnimNames = { g_store1 };
static CCharStringVector g_ConversationAttitudeAnimNames = { g_store2 };
static CCharStringVector g_ConversationAttitudeAnimGroupNames = { g_store3 };

static void Init()
{
    if (!g_ConversationAttitudeInitialised) {
        g_AttitudeAnimNames[0] = "STANDARD_TALK_GENERIC";
        g_AttitudeAnimNames[1] = "STANDARD_TALK_GENERIC";
        g_AttitudeAnimNames[2] = "STANDARD_TALK_LOVE";
        g_AttitudeAnimNames[3] = "STANDARD_TALK_GENERIC";
        g_AttitudeAnimNames[4] = "STANDARD_TALK_GENERIC";
        g_AttitudeAnimNames[5] = "STANDARD_TALK_FEAR";
        g_AttitudeAnimNames[6] = "STANDARD_TALK_ANGER";
        g_AttitudeAnimNames[7] = "STANDARD_TALK_ANGER";
        g_AttitudeAnimNames[8] = "STANDARD_TALK_ANGER";
        g_AttitudeAnimNames[9] = "STANDARD_TALK_ANGER";
        g_AttitudeAnimNames[10] = "STANDARD_TALK_LOVE";
        g_AttitudeAnimNames[11] = "STANDARD_TALK_GENERIC";
        g_AttitudeAnimNames[12] = "STANDARD_TALK_ANGER";
        g_AttitudeAnimNames[13] = "STANDARD_TALK_FEAR";
        g_AttitudeAnimNames[14] = "STANDARD_TALK_ANGER";
        g_AttitudeAnimNames[15] = "STANDARD_TALK_ANGER";
        g_AttitudeAnimNames[16] = "STANDARD_TALK_LOVE";
        g_AttitudeAnimNames[17] = "STANDARD_TALK_GENERIC";
        g_ConversationAttitudeAnimNames[0] = "NULL";
        g_ConversationAttitudeAnimNames[1] = "STANDARD_TALK_GENERIC";
        g_ConversationAttitudeAnimNames[2] = "STANDARD_TALK_LOVE";
        g_ConversationAttitudeAnimNames[3] = "STANDARD_TALK_ANGER";
        g_ConversationAttitudeAnimNames[4] = "STANDARD_TALK_FEAR";
        g_ConversationAttitudeAnimNames[5] = "STANDARD_TALK_GLEE";
        g_ConversationAttitudeAnimNames[6] = "STANDARD_TALK_SADNESS";
        g_ConversationAttitudeAnimNames[7] = "STANDARD_TALK_RIDICULE";
        g_ConversationAttitudeAnimNames[8] = "STANDARD_TALK_FRIENDLY";
        g_ConversationAttitudeAnimNames[9] = "STANDARD_TALK_RIDICULE";
        g_ConversationAttitudeAnimNames[10] = "STANDARD_TALK_GENERIC";
        g_ConversationAttitudeAnimNames[11] = "STANDARD_TALK_GENERIC";
        g_ConversationAttitudeAnimGroupNames[0] = "";
        g_ConversationAttitudeAnimGroupNames[1] = "CONVERSATION_HAPPY";
        g_ConversationAttitudeAnimGroupNames[2] = "CONVERSATION_HAPPY";
        g_ConversationAttitudeAnimGroupNames[3] = "CONVERSATION_ANGRY";
        g_ConversationAttitudeAnimGroupNames[4] = "CONVERSATION_FEAR";
        g_ConversationAttitudeAnimGroupNames[5] = "CONVERSATION_HAPPY";
        g_ConversationAttitudeAnimGroupNames[6] = "CONVERSATION_SAD";
        g_ConversationAttitudeAnimGroupNames[7] = "CONVERSATION_BELLIGERENT";
        g_ConversationAttitudeAnimGroupNames[8] = "CONVERSATION_HAPPY";
        g_ConversationAttitudeAnimGroupNames[9] = "CONVERSATION_BELLIGERENT";
        g_ConversationAttitudeAnimGroupNames[10] = "CONVERSATION_CAGED";
        g_ConversationAttitudeAnimGroupNames[11] = "CONVERSATION_HOLDING_SWORD";
        g_ConversationAttitudeInitialised = true;
    }
}

int main()
{
    int ok = 1;
    Init();
    if (std::strcmp(g_store1[2].text, "STANDARD_TALK_LOVE") != 0 || std::strcmp(g_store1[17].text, "STANDARD_TALK_GENERIC") != 0 ||
        std::strcmp(g_store1[13].text, "STANDARD_TALK_FEAR") != 0) ok = 0;
    if (std::strcmp(g_store2[0].text, "NULL") != 0 || std::strcmp(g_store2[8].text, "STANDARD_TALK_FRIENDLY") != 0) ok = 0;
    if (std::strcmp(g_store3[0].text, "") != 0 || std::strcmp(g_store3[11].text, "CONVERSATION_HOLDING_SWORD") != 0 ||
        std::strcmp(g_store3[7].text, "CONVERSATION_BELLIGERENT") != 0) ok = 0;
    for (int i = 0; i < 18; ++i) if (g_store1[i].assigns != 1) ok = 0;
    for (int i = 0; i < 12; ++i) if (g_store2[i].assigns != 1 || g_store3[i].assigns != 1) ok = 0;
    if (!g_ConversationAttitudeInitialised) ok = 0;
    g_store1[0] = "CHANGED";
    Init();                                                   // already initialised: nothing is reassigned
    if (std::strcmp(g_store1[0].text, "CHANGED") != 0 || g_store2[5].assigns != 1) ok = 0;
    if (!ok) { std::printf("004cd670_TEST FAIL\n"); return 1; }
    std::printf("004cd670_TEST PASS\n");
    return 0;
}