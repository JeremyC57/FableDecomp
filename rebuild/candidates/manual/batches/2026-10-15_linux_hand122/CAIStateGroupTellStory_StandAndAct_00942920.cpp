// CAIStateGroup_TellStory::StandAndAct(long) -- play the next story-telling gesture from a cyclic table and wait for it
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00942920, length 147 bytes (+ int3 padding).
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0, CCreatureAction_PlayAnimation::
// CCreatureAction_PlayAnimation(CThingCreatureBase*, const CCharString&, const CSoundPair&, bool, bool, bool,
// long, long) @ 0x008424F0, CAIStateGroupBase::AIAction_PerformActionAndWait(CCreatureActionBase*, bool)
// @ 0x0097DB70, CCreatureActionBase::~CCreatureActionBase @ 0x00693EF0, CCharString::~CCharString @ 0x0099EAE0.
// Globals: the gesture cursor @ 0x013BB718 and the null-terminated gesture-name table @ 0x0138CCF0.
//
// Shape: the cursor is pre-incremented and wrapped to 0 when it lands on the table's null terminator (the
// incremented value stays in eax for both the test and the reload), the selected name builds a named
// CCharString, then the landed 0x00947CA0 animation action with the WAIT call (`false` pushed first; here as
// immediates because ecx holds PBrain), frame 0xc0 = string 4 + sound pair 8 + action 0xb4. The 4-byte
// parameter is unused (`ret 4`). No engine header exists for this class.
class CThingCreatureBase;

struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length);     // 0x0099EBF0
    ~CCharString();                                 // 0x0099EAE0
};

struct CSoundPair {
    long m_first;
    long m_second;
    CSoundPair() : m_first(0), m_second(0) { }
};

struct CCreatureActionBase {
    virtual void v00();
    unsigned char _body[0xa4];                      // +0x04..+0xa7
    ~CCreatureActionBase();                         // 0x00693EF0
};

struct CCreatureAction_PlayAnimation : CCreatureActionBase {
    unsigned char _anim[0x0c];                      // +0xa8..+0xb3
    CCreatureAction_PlayAnimation(CThingCreatureBase* creature, const CCharString& animation,
        const CSoundPair& sound, bool a, bool b, bool c, long d, long e);   // 0x008424F0
};

struct CAIBrain {
    unsigned char _pad[0x18];
    CThingCreatureBase* PCreature;                  // +0x18
};

struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                               // +0x04
    unsigned char _pad[0x0c];                       // +0x08..+0x13
    CThingCreatureBase* GetCreature() const { return PBrain->PCreature; }
    long AIAction_PerformActionAndWait(CCreatureActionBase* action, bool wait);   // 0x0097DB70
};

extern long g_storyGestureIndex;                    // 0x013BB718
extern const char* g_storyGestures[];               // 0x0138CCF0 (null-terminated)

struct CAIStateGroup_TellStory : CAIStateGroupBase {
    void StandAndAct(long unused);
};

void CAIStateGroup_TellStory::StandAndAct(long unused)
{
    if (g_storyGestures[++g_storyGestureIndex] == 0)
        g_storyGestureIndex = 0;
    CCharString animation(g_storyGestures[g_storyGestureIndex], -1);
    AIAction_PerformActionAndWait(&CCreatureAction_PlayAnimation(GetCreature(), animation, CSoundPair(),
        false, true, false, 0, 0), false);
}
