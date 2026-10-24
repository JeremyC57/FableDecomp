// NCreatureMode::CModeBriarRoseTeleportIntangible::FrameUpdate() -- base update, then count the intangibility timer down and at zero start the "concentrate into" action on a live target
// Leaf: ?FrameUpdate@CModeBriarRoseTeleportIntangible
// Retail: Fable.exe (TLC, VC7.1) @ 0x008E8940, 137 bytes (+ int3 padding), `ret`; vtable 0x0128026C slot 3.
// Callees: CModeBase::FrameUpdate @ 0x0073AC50 (qualified), CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (three times),
//   CCreatureAction_BriarRoseConcentrateInto(CThingCreatureBase*, CBaseObject*, bool) @ 0x008E8BB0 (landed; retail sizeof
//   0x11d, byte-packed: Imposter +0x114, PTarget +0x115), CThingCreatureBase::SetCurrentAction @ 0x006644F0,
//   ~CBaseIntelligentPointer @ 0x00A01C10, ~CCombatActionBase @ 0x0062E900.
// Name: FableWin.pdb `?FrameUpdate@CModeBriarRoseTeleportIntangible@NCreatureMode@@UAEXXZ` (slot 3).
// Layout: Ego_r.pdb CModeBriarRoseTeleportIntangible: CModeStandBase 0x68, Filter +0x68, Timer +0x74, PTarget +0x78.
// Shape: the landed sibling CModeBriarRoseTeleportInvulnerable::FrameUpdate @ 0x008E8830: the action temporary sits in the
//   `&&` chain of the if, so VC7.1 keeps a construction flag (zeroed in the frame at entry, live in bl) and destroys the
//   temporary only when it was built: the generated action dtor runs the PTarget pointer dtor then ~CCombatActionBase.
//   Nothing follows, so the SetCurrentAction result is dropped. Only the intelligent-pointer classes are byte-packed (the
//   action keeps its 4-byte alignment, so the temporary sits at frame +4 after the flag dword, as retail).
struct CBaseObject;
struct CCreatureActionBase;

struct CThing {
    unsigned char _pad00[0x91];
    unsigned char m_flags91;               // +0x91 bit 0: dead
};
struct CThingCreatureBase : CThing {
    bool SetCurrentAction(const CCreatureActionBase& action);   // 0x006644F0
};

#pragma pack(push, 1)
struct CBaseIntelligentPointer {           // vtable + PData (8 bytes; byte-aligned so it can sit at an odd offset)
    void* PData;
    CBaseIntelligentPointer();             // 0x00A01B10
    virtual ~CBaseIntelligentPointer();    // 0x00A01C10
    CThingCreatureBase* GetPItem() const;  // 0x00A01B50
};
struct CIntelligentPointer : CBaseIntelligentPointer {
};
#pragma pack(pop)

struct CCreatureActionBase {               // retail sizeof 0xa8
    virtual void v00();
    unsigned char _body[0xa4];
};
struct CCombatActionBase : CCreatureActionBase {   // retail sizeof 0x114
    unsigned char _combat[0x6c];
    ~CCombatActionBase();                  // 0x0062E900
};
struct CCreatureAction_BriarRoseConcentrateInto : CCombatActionBase {   // retail sizeof 0x11d
    unsigned char       Imposter;          // +0x114
    CIntelligentPointer PTarget;           // +0x115
    CCreatureAction_BriarRoseConcentrateInto(CThingCreatureBase* owner, CBaseObject* target, bool imposter);   // 0x008E8BB0
};

struct CModeBase {                         // CTCCreatureModeManager::CModeBase, retail 0x1c
    virtual void v00();
    CThingCreatureBase* ParentThing;       // +0x04
    unsigned char       _body[0x14];       // WeaponBasedFilter, FramesBeforeRemovingSelf, Active
    void FrameUpdate();                    // 0x0073AC50
};
struct CModeStandBase : CModeBase {        // retail sizeof 0x68
    unsigned char _stand[0x4c];
};

namespace NCreatureMode {
struct CModeBriarRoseTeleportIntangible : CModeStandBase {   // vtable 0x0128026C
    unsigned char           Filter[0x0c];  // +0x68 CAnimFilterOfGroup
    long                    Timer;         // +0x74
    CBaseIntelligentPointer PTarget;       // +0x78 CIntelligentPointer<CThingCreatureBase>
    void FrameUpdate();
};
}

void NCreatureMode::CModeBriarRoseTeleportIntangible::FrameUpdate()
{
    CModeBase::FrameUpdate();
    if (--Timer <= 0) {
        CThingCreatureBase* owner = ParentThing;
        if (PTarget.GetPItem() != 0 && !(PTarget.GetPItem()->m_flags91 & 1) &&
            owner->SetCurrentAction(CCreatureAction_BriarRoseConcentrateInto(owner, (CBaseObject*)PTarget.GetPItem(), false)))
            return;
    }
}