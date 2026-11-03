// CConversationAttitude::Init() -- one-shot fill of the three attitude-name tables: the talk animation for each EAttitude, the talk animation and the conversation anim group for each EConversationAttitude
// Leaf: ?Init@CConversationAttitude@@SIXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004CD670, 810 bytes, static __fastcall, plain `ret`. Ego_r.pdb
//   `?Init@CConversationAttitude@@SIXXZ` (with GetAnimName(EAttitude), GetAnimName(EConversationAttitude) and
//   GetAnimGroupName(EConversationAttitude) reading the three tables); catalog "Init" (name only).
// Callees: CCharString::operator=(const char*) @ 0x0099EFE0, 42 times. Globals: the initialised flag @ 0x013B8A28 and three
//   STLport vector<CCharString> tables @ 0x013B8A2C / 0x013B8A38 / 0x013B8A44 (12 bytes apart; _M_start reloaded per store).
// Naming wave 16 (c674): the 0x013B8A38 / 0x013B8A44 tables are the static members
//   CConversationAttitude::ConversationAttitudeToAnimName / ConversationAttitudeToAnimGroupName (Ego_r.pdb dynamic initializer
//   and atexit names; the landed initializers 0x0121A4F0 / 0x0121A580 define them); the landed g_* externs are respelled.
// Strings: every literal pairs with its retail push in statement order (generated from the retail disassembly;
//   duplicates pool to one address as in retail).
class CCharString {
public:
    void* PStringData;
    const CCharString& operator=(const char* text);          // 0x0099EFE0
};

namespace std {
template <class _Tp> class allocator { public: allocator() { } };
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector {                                         // STLport 4.6: 12 bytes
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    _Tp& operator[](unsigned int __n) { return *(_M_start + __n); }
};
}

extern bool g_ConversationAttitudeInitialised;                            // 0x013B8A28
extern std::vector<CCharString> g_AttitudeAnimNames;                     // 0x013B8A2C (18, by EAttitude)

class CConversationAttitude {
public:
    static void __fastcall Init();
    static std::vector<CCharString> ConversationAttitudeToAnimName;        // 0x013B8A38 (12, by EConversationAttitude)
    static std::vector<CCharString> ConversationAttitudeToAnimGroupName;   // 0x013B8A44 (12, by EConversationAttitude)
};

void __fastcall CConversationAttitude::Init()
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
        ConversationAttitudeToAnimName[0] = "NULL";
        ConversationAttitudeToAnimName[1] = "STANDARD_TALK_GENERIC";
        ConversationAttitudeToAnimName[2] = "STANDARD_TALK_LOVE";
        ConversationAttitudeToAnimName[3] = "STANDARD_TALK_ANGER";
        ConversationAttitudeToAnimName[4] = "STANDARD_TALK_FEAR";
        ConversationAttitudeToAnimName[5] = "STANDARD_TALK_GLEE";
        ConversationAttitudeToAnimName[6] = "STANDARD_TALK_SADNESS";
        ConversationAttitudeToAnimName[7] = "STANDARD_TALK_RIDICULE";
        ConversationAttitudeToAnimName[8] = "STANDARD_TALK_FRIENDLY";
        ConversationAttitudeToAnimName[9] = "STANDARD_TALK_RIDICULE";
        ConversationAttitudeToAnimName[10] = "STANDARD_TALK_GENERIC";
        ConversationAttitudeToAnimName[11] = "STANDARD_TALK_GENERIC";
        ConversationAttitudeToAnimGroupName[0] = "";
        ConversationAttitudeToAnimGroupName[1] = "CONVERSATION_HAPPY";
        ConversationAttitudeToAnimGroupName[2] = "CONVERSATION_HAPPY";
        ConversationAttitudeToAnimGroupName[3] = "CONVERSATION_ANGRY";
        ConversationAttitudeToAnimGroupName[4] = "CONVERSATION_FEAR";
        ConversationAttitudeToAnimGroupName[5] = "CONVERSATION_HAPPY";
        ConversationAttitudeToAnimGroupName[6] = "CONVERSATION_SAD";
        ConversationAttitudeToAnimGroupName[7] = "CONVERSATION_BELLIGERENT";
        ConversationAttitudeToAnimGroupName[8] = "CONVERSATION_HAPPY";
        ConversationAttitudeToAnimGroupName[9] = "CONVERSATION_BELLIGERENT";
        ConversationAttitudeToAnimGroupName[10] = "CONVERSATION_CAGED";
        ConversationAttitudeToAnimGroupName[11] = "CONVERSATION_HOLDING_SWORD";
        g_ConversationAttitudeInitialised = true;
    }
}