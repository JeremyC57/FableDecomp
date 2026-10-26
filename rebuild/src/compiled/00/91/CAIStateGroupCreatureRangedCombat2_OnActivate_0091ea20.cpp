// CAIStateGroup_CreatureRangedCombat2::OnActivate() -- register the creature against its live target
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0091EA20, length 68 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (three times on PTargetThing, +0x14),
// CThingCreatureBase::GetCombatManager @ 0x00661ED0, CCombatManager::RegisterCombatant
// (CThingCreatureBase* combatant, CThingCreatureBase* target, bool) @ 0x006EDC50.
// Arguments right to left: `false`, the target (GetPItem), the creature (PBrain -> Creature,
// read into esi, replacing `this` which is no longer needed), then the combat manager object
// expression. The target's byte at +0x91 bit 0 is its "dead" flag (`test byte ptr [eax+91h],1`),
// a one-bit bitfield. PTargetThing's address lives in edi across the calls.
class CThingCreatureBase;
class CCombatManager { public:
    void RegisterCombatant(CThingCreatureBase* combatant, CThingCreatureBase* target, bool hostile);   // 0x006EDC50
};

class CThingCreatureBase { public:
    unsigned char _pad[0x91];
    unsigned char IsDead : 1;               // +0x91 bit 0
    CCombatManager* GetCombatManager();     // 0x00661ED0
};

struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;                          // +4
    CThingCreatureBase* GetPItem() const;   // 0x00A01B50
};

struct CIntelligentPointer : CBaseIntelligentPointer {
};

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThingCreatureBase* Creature;           // +0x18
};

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                       // +0x04
    unsigned char _pad[0xc];                // +0x08..+0x13
};

struct CAIStateGroup_EnemyCreatureBase : CAIStateGroupBase {
    CIntelligentPointer PTargetThing;       // +0x14
};

struct CAIStateGroup_CreatureRangedCombat2 : CAIStateGroup_EnemyCreatureBase {
    bool AddedStrafe;                       // +0x1c
    virtual void OnActivate();
};

void CAIStateGroup_CreatureRangedCombat2::OnActivate()
{
    if (PTargetThing.GetPItem() != 0 && !PTargetThing.GetPItem()->IsDead) {
        CThingCreatureBase* creature = PBrain->Creature;
        creature->GetCombatManager()->RegisterCombatant(creature, PTargetThing.GetPItem(), false);
    }
}