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
class CThingAICreature;

class CThing { public:
    unsigned char _pad[0x91];
    unsigned char m_flags91;                // +0x91 bit 0: dead
};

class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return reinterpret_cast<T*>(CBaseIntelligentPointer::GetPItem()); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem(reinterpret_cast<CBaseObject*>(item)); }
};

class CCreatureActionBase { public:
    virtual void v00();
    unsigned char _body[0xa4];              // +0x04..+0xa7
    virtual ~CCreatureActionBase();                 // 0x00693EF0
};

namespace N_00933cc0 { struct CCreatureAction_AlertToPlayer : CCreatureActionBase {
    CCreatureAction_AlertToPlayer(CThingAICreature* creature);     // 0x00933CC0
}; } using N_00933cc0::CCreatureAction_AlertToPlayer;

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThingAICreature* Creature;             // +0x18
};

enum EAIActionReturn { };
class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                       // +0x04
    unsigned char _pad[0xc];                protected: // +0x08..+0x13
    EAIActionReturn AIAction_PerformActionAndWait(const CCreatureActionBase& action, bool wait); public:   // 0x0097DB70
    void Terminate();                       // 0x008FCF40
};

struct CAIStateGroup_MinionMotherProtectFamily : CAIStateGroupBase {
    CIntelligentPointer<CThing> PFamilyMember;  // +0x14 CIntelligentPointer<CThingCreatureBase>
    virtual void Update();
};

void CAIStateGroup_MinionMotherProtectFamily::Update()
{
    if (PFamilyMember.GetPItem() != 0 && !(PFamilyMember.GetPItem()->m_flags91 & 1)) {
        CThingAICreature* creature = PBrain->Creature;
        long result = AIAction_PerformActionAndWait(CCreatureAction_AlertToPlayer(creature), false);
        if (result == 2) {
            Terminate();
            return;
        }
    }
    Terminate();
}