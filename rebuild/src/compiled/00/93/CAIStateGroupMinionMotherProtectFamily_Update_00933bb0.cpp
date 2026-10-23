// CAIStateGroup_MinionMotherProtectFamily::Update() -- while the protected family member is alive, perform the
// "alert to player" action and wait for it; then terminate.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00933BB0, length 93 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PFamilyMember at +0x14, twice: null test, then the
// dead flag), CCreatureAction_AlertToPlayer::CCreatureAction_AlertToPlayer(CThingAICreature*) @ 0x00933CC0,
// CAIStateGroupBase::AIAction_PerformActionAndWait(CCreatureActionBase*, bool) @ 0x0097DB70, the action base
// destructor @ 0x00693EF0 (manifest "CPhysicsMeshInfo::~CPhysicsMeshInfo" is a propagated label: it is the
// out-of-line destructor of the 0xa8-byte action base), CAIStateGroupBase::Terminate @ 0x008FCF40.
// Shape: the landed CreatureGoUnderground shape -- the action is a TEMPORARY inside the call expression
// (`false` pushed first, the constructor's returned `this` pushed as the action, the base destructor after the
// call; frame = sizeof(action) = 0xa8), the creature read into a named local first. The action's result is
// kept in edi across the destructor (retail's `mov edi,eax`) although nothing reads it: the source tests it
// (`if (result == 2) { Terminate(); return; }`) and VC7.1 merges that Terminate with the shared one AFTER
// register allocation, so the compare disappears but the callee-saved copy stays (a plain dropped result
// emits no `mov edi,eax`). The dead flag is bit 0 of the thing's byte at +0x91. No engine header for this class; layout local, names from the
// PDB pattern (PFamilyMember descriptive).
struct CThingAICreature;

struct CThing {
    unsigned char _pad[0x91];
    unsigned char m_flags91;                // +0x91 bit 0: dead
};

struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;
    CThing* GetPItem();                     // 0x00A01B50
};

struct CCreatureActionBase {
    virtual void v00();
    unsigned char _body[0xa4];              // +0x04..+0xa7
    ~CCreatureActionBase();                 // 0x00693EF0
};

namespace N_00933cc0 { struct CCreatureAction_AlertToPlayer : CCreatureActionBase {
    CCreatureAction_AlertToPlayer(CThingAICreature* creature);     // 0x00933CC0
}; } using N_00933cc0::CCreatureAction_AlertToPlayer;

struct CAIBrain {
    unsigned char _pad[0x18];
    CThingAICreature* Creature;             // +0x18
};

struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                       // +0x04
    unsigned char _pad[0xc];                // +0x08..+0x13
    long AIAction_PerformActionAndWait(CCreatureActionBase* action, bool wait);   // 0x0097DB70
    void Terminate();                       // 0x008FCF40
};

struct CAIStateGroup_MinionMotherProtectFamily : CAIStateGroupBase {
    CBaseIntelligentPointer PFamilyMember;  // +0x14 CIntelligentPointer<CThingCreatureBase>
    virtual void Update();
};

void CAIStateGroup_MinionMotherProtectFamily::Update()
{
    if (PFamilyMember.GetPItem() != 0 && !(PFamilyMember.GetPItem()->m_flags91 & 1)) {
        CThingAICreature* creature = PBrain->Creature;
        long result = AIAction_PerformActionAndWait(&CCreatureAction_AlertToPlayer(creature), false);
        if (result == 2) {
            Terminate();
            return;
        }
    }
    Terminate();
}