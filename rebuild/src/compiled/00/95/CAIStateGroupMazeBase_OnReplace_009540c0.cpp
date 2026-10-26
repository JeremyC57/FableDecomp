// CAIStateGroup_MazeBase::OnReplace() -- when another group takes over, pull the creature out of the combat
// manager if it is registered as a combatant.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009540C0, length 44 bytes (+ int3 padding).
// Callees: the creature's IsRegisteredCombatant query @ 0x00662410 (manifest "C3DAnimationBlendState::
// AddCombinationBlend" is a propagated label; it is a no-argument bool on the creature), CAIStateGroupBase::
// GetPWorld @ 0x008FCFC0 (PBrain->ThingManager->World), CCombatManager::DeregisterCombatant(CThingCreatureBase*)
// @ 0x006ED9C0 on the world's combat manager at +0x4c.
// Shape: the creature is read into a named local BEFORE the world call (retail keeps it in edi across
// GetPWorld and pushes it afterwards); the world itself is a named local too, which is what keeps the
// creature push after the call instead of before it. Base layout modelled locally (PDB names).
class CThingCreatureBase { public:
public:
    bool IsInMeleeAsAttacker() const;   // 0x00662410
};

class CCombatManager { public:
    void DeregisterCombatant(CThingCreatureBase* creature);   // 0x006ED9C0
};

class CWorld { public:
    unsigned char _pad[0x4c];
    CCombatManager* m_pCombatManager;       // +0x4c
};

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThingCreatureBase* Creature;           // +0x18
};

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                       // +0x04
    unsigned char _pad[0xc];                // +0x08..+0x13
    CWorld* GetPWorld();                    // 0x008FCFC0
};

struct CAIStateGroup_MazeBase : CAIStateGroupBase {
    virtual void OnReplace();
};

void CAIStateGroup_MazeBase::OnReplace()
{
    if (PBrain->Creature->IsInMeleeAsAttacker()) {
        CThingCreatureBase* creature = PBrain->Creature;
        CWorld* world = GetPWorld();
        world->m_pCombatManager->DeregisterCombatant(creature);
    }
}