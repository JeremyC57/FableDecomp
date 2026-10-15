// CAIStateGroup_HoverHornetQueen::PlayIdle() -- play the SPECIAL_BOAST combat animation and wait for it, then terminate unless the group is still runnable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0095B680, length 102 bytes (+ int3 padding).
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0,
// CCreatureAction_PlayCombatAnimation::CCreatureAction_PlayCombatAnimation(CThingCreatureBase*, const CCharString&,
// bool) @ 0x008B7FF0, CAIStateGroupBase::AIAction_PerformActionAndWait(CCreatureActionBase*, bool) @ 0x0097DB70,
// CCombatActionBase::~CCombatActionBase @ 0x0062E900 (the implicit destructor of the derived action; manifest
// "CTCLightningOrb::~CTCLightningOrb" is a propagated label -- the Kraken rows 0x009250F0.. call it after the
// out-of-line CCombatActionBase constructor 0x00858030), CCharString::~CCharString @ 0x0099EAE0,
// CAIStateGroup_HoverHornetQueen::IsStillRunnable @ 0x0095A420, CAIStateGroupBase::Terminate @ 0x008FCF40.
//
// Shape: the animation name is a named CCharString in an inner block (built before any push, destroyed
// before the IsStillRunnable test); `false` is pushed before the action temporary is built inside the
// PerformActionAndWait call (frame 0x11c = string 4 + action 0x118), the base destructor runs after the call. The creature comes through an inline GetCreature() accessor. No engine header
// covers the action; the state group's header is flattened data-only: local model.
class CThingCreatureBase;

struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length);     // 0x0099EBF0
    ~CCharString();                                 // 0x0099EAE0
};

struct CCreatureActionBase {
    virtual void v00();
    unsigned char _body[0xa4];                      // +0x04..+0xa7
};

struct CCombatActionBase : CCreatureActionBase {
    unsigned char _combat[0x6c];                    // +0xa8..+0x113
    ~CCombatActionBase();                           // 0x0062E900
};

struct CCreatureAction_PlayCombatAnimation : CCombatActionBase {
    long m_loop;                                    // +0x114 (descriptive)
    CCreatureAction_PlayCombatAnimation(CThingCreatureBase* creature, const CCharString& animation, bool loop);   // 0x008B7FF0
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
    void Terminate();                               // 0x008FCF40
};

struct CAIStateGroup_HoverHornetQueen : CAIStateGroupBase {
    bool IsStillRunnable();                         // 0x0095A420
    void PlayIdle();
};

void CAIStateGroup_HoverHornetQueen::PlayIdle()
{
    {
        CCharString animation("SPECIAL_BOAST", -1);
        AIAction_PerformActionAndWait(&CCreatureAction_PlayCombatAnimation(GetCreature(), animation, true), false);
    }
    if (!IsStillRunnable())
        Terminate();
}