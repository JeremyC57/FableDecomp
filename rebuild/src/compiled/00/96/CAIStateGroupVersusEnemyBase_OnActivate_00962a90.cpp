// CAIStateGroup_VersusEnemyBase::OnActivate() -- register the creature as a combatant against its target
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00962A90, length 88 bytes (+ int3 padding).
// Callees: CThingCreatureBase::IsRegisteredCombatant() @ 0x00662410 (on PBrain->Creature: asks the
// world's combat manager; the manifest's "AddCombinationBlend" label is propagated),
// CBaseIntelligentPointer::GetPItem @ 0x00A01B50 three times on PTargetThing (+0x14, engine header
// CAIStateGroup_VersusEnemyBase.h): the inlined IS_THING_ALIVE (null, then dead bit +0x91 & 1)
// and the argument fetch, CAIStateGroupBase::GetPWorld @ 0x008FCFC0 (PBrain -> +0x1c -> +0x24),
// CCombatManager::RegisterCombatant(CThingCreatureBase*, CThingCreatureBase*, bool) @ 0x006EDC50
// on the world's combat manager at +0x4c.
// Shape: nothing happens when the creature is already registered; otherwise, with a live target,
// the creature (named local, ebx -- read before the GetPWorld call) and the target item are
// registered with `true`. The combat manager is a second named local (esi) so it is fetched before
// the right-to-left argument pushes (as a bare object expression it would be evaluated after them).
struct CThing {
    unsigned char _pad[0x91];
    unsigned char m_flags;                      // +0x91 bit 0: dead
};

struct CThingAICreature {
    bool IsRegisteredCombatant();               // 0x00662410
};

struct CCombatManager {
    void RegisterCombatant(CThingAICreature* creature, CThing* target, bool attacker);    // 0x006EDC50
};

struct CWorld {
    unsigned char _pad[0x4c];
    CCombatManager* m_pCombatManager;           // +0x4c
};

struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;                              // +0x04
    CThing* GetPItem();                         // 0x00A01B50
    __forceinline bool IsAlive()
    {
        if (GetPItem() != 0) {
            if (!(GetPItem()->m_flags & 1))
                return true;
        }
        return false;
    }
};

struct CAIBrain {
    unsigned char _pad[0x18];
    CThingAICreature* Creature;                 // +0x18
};

struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
    CWorld* GetPWorld();                        // 0x008FCFC0
};

struct CAIStateGroup_VersusEnemyBase : CAIStateGroupBase {
    CBaseIntelligentPointer PTargetThing;       // +0x14 CIntelligentPointer<CThingCreatureBase>
    virtual void OnActivate();
};

void CAIStateGroup_VersusEnemyBase::OnActivate()
{
    if (!PBrain->Creature->IsRegisteredCombatant()) {
        if (PTargetThing.IsAlive()) {
            CThingAICreature* creature = PBrain->Creature;
            CCombatManager* combatManager = GetPWorld()->m_pCombatManager;
            combatManager->RegisterCombatant(creature, PTargetThing.GetPItem(), true);
        }
    }
}