// CAIStateGroup_MazeTutorialBattleUseShield::Update() -- while the target lives and the group runs, raise the will shield (3s, 5s) and stamp FrameOfLastShield; then terminate
// Leaf: ?Update@CAIStateGroup_MazeTutorialBattleUseShield@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00956460, length 127 bytes.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (x2, PTargetThing +0x14), CAIStateGroupBase::IsStopping
// @ 0x008FD010, CCombatAction_WillShieldInto::CCombatAction_WillShieldInto(CThingCreatureBase*, float, float) @ 0x00956760
// (vtable 0x0129461C; manifest "CCombatAction_WillEnflameInto" is a sibling label), CAIStateGroupBase::
// AIAction_PerformActionAndWait(CCreatureActionBase*, bool) @ 0x0097DB70, CCombatActionBase::~CCombatActionBase
// @ 0x0062E900, the world accessor @ 0x008FCFC0, CWorld::GetCurrentFrame @ 0x0049D870 (static), CAIStateGroupBase::
// Terminate @ 0x008FCF40.
//
// Shape: `if (IS_THING_ALIVE(PTargetThing) && !IsStopping()) { PerformActionAndWait(&WillShieldInto(creature,
// 3.0f, 5.0f), false); FrameOfLastShield = GetPWorld()->GetCurrentFrame(); } Terminate();` -- `false` pushed
// before the out-of-line action constructor's arguments (the floats as immediates), the action's implicit
// destructor is the base one, the creature comes through an inline GetCreature() (a direct PBrain->PCreature
// argument loads it into ecx instead of eax), the static frame getter still calls the world accessor. Frame 0x11c = the action.
// PDB: FrameOfLastShield +0x20. The engine header is flattened data-only: local model.
class CThingCreatureBase;

#include "engine/CThing.h"

class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"


inline bool IS_THING_ALIVE(const CIntelligentPointer<CThing>& p)
{
    if (p.Get() != 0) {
        if (!p.Get()->IsDead())
            return true;
    }
    return false;
}

class CCreatureActionBase { public:
    virtual void v00();
    unsigned char _body[0xa4];                  // +0x04..+0xa7
};

class CCombatActionBase : public CCreatureActionBase { public:
    unsigned char _combat[0x6c];                // +0xa8..+0x113
    virtual ~CCombatActionBase();                       // 0x0062E900
};

class CCombatAction_WillShieldInto : public CCombatActionBase { public:
    float m_first;                              // +0x114
    float m_second;                             // +0x118
    CCombatAction_WillShieldInto(CThingCreatureBase* creature, float first, float second);   // 0x00956760
};

class CWorld { public:
    static long __fastcall GetFrame();              // 0x0049D870
};

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThingCreatureBase* PCreature;              // +0x18
};

enum EAIActionReturn { };
class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad[0x0c];                   // +0x08..+0x13
    CThingCreatureBase* GetCreature() const { return PBrain->PCreature; }
    CWorld* GetPWorld();                        // 0x008FCFC0
protected:
    bool IsStopping() const;                          // 0x008FD010 (Ego_r.pdb protected const)
    EAIActionReturn AIAction_PerformActionAndWait(const CCreatureActionBase& action, bool wait); public:   // 0x0097DB70
    void Terminate();                           // 0x008FCF40
};

struct CAIStateGroup_MazeTutorialBattleUseShield : CAIStateGroupBase {
    CIntelligentPointer<CThing> PTargetThing;       // +0x14 CIntelligentPointer<CThingCreatureBase>
    long _base1c;                               // +0x1c
    long FrameOfLastShield;                     // +0x20
    void Update();
};

void CAIStateGroup_MazeTutorialBattleUseShield::Update()
{
    if (IS_THING_ALIVE(PTargetThing) && !IsStopping()) {
        AIAction_PerformActionAndWait(CCombatAction_WillShieldInto(GetCreature(), 3.0f, 5.0f), false);
        FrameOfLastShield = GetPWorld()->GetFrame();
    }
    Terminate();
}