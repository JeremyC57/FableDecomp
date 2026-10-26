// CAIStateGroup_GoToPub::Laugh() -- play the SEATED_TAVERN_LAUGH animation and report whether it completed
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00947D30, length 114 bytes (+ int3 padding).
// Callees: CCharString::CCharString(const char*, long)
// @ 0x0099EBF0, CCreatureAction_PlayAnimation::CCreatureAction_PlayAnimation(CThingCreatureBase*,
// const CCharString&, const CSoundPair&, bool, bool, bool, long, long) @ 0x008424F0,
// CAIStateGroupBase::AIAction_PerformAction(CCreatureActionBase*) @ 0x0097D8B0, the action base destructor
// @ 0x00693EF0 (manifest "CPhysicsMeshInfo::~CPhysicsMeshInfo" is propagated; it is the 0xa8-byte
// CCreatureActionBase destructor), CCharString::~CCharString @ 0x0099EAE0.
//
// Shape: the SitIdle twin (0x00947CA0) without the hands-off test; the result is `bool failed = .. != 2;
// return !failed;` (`setne bl` across the destructors, then `test bl,bl ; sete al`). The animation name is a NAMED
// CCharString local (built before any argument push), the CSoundPair() temporary is zeroed in its frame slot
// between the pushes (ecx is the zero register), and the action is a temporary inside the PerformAction call (its implicit destructor
// is the base destructor; frame 0xc0 = string 4 + sound pair 8 + action 0xb4). The `!= 2` flag rides in bl
// across both destructors. The creature comes through an inline GetCreature() accessor: a direct
// `PBrain->PCreature` argument swaps the eax/ecx/edx roles (brain in edx, zero in eax). No engine header for CAIStateGroup_GoToPub: local model (PDB names).
class CThingCreatureBase;

class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length);     // 0x0099EBF0
    ~CCharString();                                 // 0x0099EAE0
};

class CSoundPair { public:
    long m_first;
    long m_second;
    CSoundPair() : m_first(0), m_second(0) { }
};

class CCreatureActionBase { public:
    virtual void v00();
    unsigned char _body[0xa4];                      // +0x04..+0xa7
    virtual ~CCreatureActionBase();                         // 0x00693EF0
};

class CCreatureAction_PlayAnimation : public CCreatureActionBase { public:
    unsigned char _anim[0x0c];                      // +0xa8..+0xb3
    CCreatureAction_PlayAnimation(CThingCreatureBase* creature, const CCharString& animation,
        const CSoundPair& sound, bool a, bool b, bool c, long d, long e);   // 0x008424F0
};

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThingCreatureBase* PCreature;                  // +0x18
};

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                               // +0x04
    unsigned char _pad[0x0c];                       // +0x08..+0x13
    CThingCreatureBase* GetCreature() const { return PBrain->PCreature; }
    long AIAction_DoNothing();                      // 0x0097E200 (EAIActionReturn)
    long AIAction_PerformAction(CCreatureActionBase* action);   // 0x0097D8B0 (EAIActionReturn)
};

struct CAIStateGroup_GoToPub : CAIStateGroupBase {
    unsigned char _members[0x8c];                   // +0x14..+0x9f (pointers, seat and bar state)
    bool HandsOffMode;                              // +0xa0
    bool UnderScriptControl;                        // +0xa1
    long TimeBeforeAskingForBeer;                   // +0xa4
    bool Laugh();
};

bool CAIStateGroup_GoToPub::Laugh()
{
    CCharString animation("SEATED_TAVERN_LAUGH", -1);
    bool failed = AIAction_PerformAction(&CCreatureAction_PlayAnimation(GetCreature(), animation,
        CSoundPair(), false, true, false, 0, 0)) != 2;
    return !failed;
}