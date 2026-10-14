// CAIStateGroup_Guildmaster::Cleanup() -- take the Guildmaster out of the combat manager if he is registered.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00977770, length 44 bytes.
// Callees: CThingCreatureBase::IsRegisteredCombatant @ 0x00662410 (manifest "C3DAnimationBlendState::
// AddCombinationBlend" is a propagated label: a bool query on the creature), CAIStateGroupBase::GetPWorld @
// 0x008FCFC0 (manifest "CPlayer::GetCurrentMode", propagated), CCombatManager::DeregisterCombatant @ 0x006ED9C0
// on the world's combat manager at CWorld+0x4c.
//
// Shape: `if (creature->IsRegisteredCombatant()) { CThingAICreature* c = PBrain->PCreature; CWorld* w =
// GetPWorld(); w->m_pCombatManager->DeregisterCombatant(c); }` -- the creature is re-read into a local (edi)
// before the GetPWorld call so it survives it, and the world must be a named local too: with the chained
// expression VC pushes the creature BEFORE the GetPWorld call (42 bytes), with the local it pushes edi after. The engine header keeps PBrain as a pad, so the base layout is local.
struct CThingAICreature {
    bool IsRegisteredCombatant();               // 0x00662410
};

struct CCombatManager {
    void DeregisterCombatant(CThingAICreature* pCreature);   // 0x006ED9C0
};

struct CWorld {
    unsigned char _pad[0x4c];
    CCombatManager* m_pCombatManager;           // +0x4c
};

struct CAIBrain {
    unsigned char _pad[0x18];
    CThingAICreature* PCreature;                // +0x18
};

struct CAIStateGroupBase {
    virtual void v00();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
    CWorld* GetPWorld();                        // 0x008FCFC0
};

struct CAIStateGroup_Guildmaster : CAIStateGroupBase {
    unsigned char _guildmaster[0x28];           // +0x14 AttackNum, PEnemy, PLastTeleportMarker, TeleportMarkers, frames
    void Cleanup();
};

void CAIStateGroup_Guildmaster::Cleanup()
{
    if (PBrain->PCreature->IsRegisteredCombatant()) {
        CThingAICreature* pCreature = PBrain->PCreature;
        CWorld* pWorld = GetPWorld();
        pWorld->m_pCombatManager->DeregisterCombatant(pCreature);
    }
}
