// Grade: DIFFER(ours v retail) -- 143v141: same gap as 0x00972110 -- retail loads the combat manager straight into ecx after the world call, then pushes the creature; the lane materialises it in eax (`mov eax,[eax+4Ch] ; push edi ; mov ecx,eax`) or pushes before the call
// CAIStateGroup_MeleeCombatNearby2::Cleanup() -- deregister the creature from the combat manager if it is a registered combatant, and drop creature mode 0x19 when the creature uses modes
// Leaf: ?Cleanup@CAIStateGroup_MeleeCombatNearby2@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0093E240, length 141 bytes (+ int3 padding); byte-identical
// shape to CAIStateGroup_JackOfBladesBase::Cleanup (0x00972110).
// Callees: CThingCreatureBase::IsRegisteredCombatant @ 0x00662410 (landed name; manifest
// "C3DAnimationBlendState::AddCombinationBlend" is propagated), CAIStateGroupBase::GetPWorld @ 0x008FCFC0,
// CCombatManager::DeregisterCombatant(CThingCreatureBase*) @ 0x006ED9C0 (world +0x4c), the creature's mode
// flag query @ 0x006621A0 (bit 1 of the byte at [this+0E0h]+148h; manifest "AreDefaultValsApplied" is
// propagated -- named descriptively), CVectorMap::LowerBound @ 0x0040F020 (inlined interface lookup, type
// 0x31 = CTCCreatureModeManager), CTCCreatureModeManager::RemoveMode(unsigned long) @ 0x0073ABA0.
//
// Shape: the creature is a named local read before the world call (edi) and the combat manager a named
// local loaded straight into ecx after it; the second block is the out-parameter GetInterface template
// with its result ignored -- VC tail-duplicates `RemoveMode(0x19)` into the hit arm and the miss arm (which
// reads the never-written manager local from the key's `push ecx` slot). No engine header models the
// state group's vptr chain: local model.
// Tried here: a state-group GetPCombatManager() inline accessor (this; same bytes as the local form). On 0x00972110:
// combat-manager local (best: world call first but the manager lands in eax + `mov ecx,eax`), the
// direct chain `GetPWorld()->m_pCombatManager->Deregister(creature)` (the lane pushes the creature before the
// world call, 139v141).
class CTCBase;

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);      // 0x0040F020
};

struct CThingCreatureBase {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    bool IsRegisteredCombatant();               // 0x00662410
    bool UsesCreatureModes();                   // 0x006621A0 (descriptive)
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};

struct CTCCreatureModeManager {                 // interface type 0x31
    void RemoveMode(unsigned long mode);        // 0x0073ABA0
};

struct CCombatManager {
    void DeregisterCombatant(CThingCreatureBase* creature);   // 0x006ED9C0
};

struct CWorld {
    unsigned char _pad[0x4c];
    CCombatManager* m_pCombatManager;           // +0x4c
};

struct CAIBrain {
    unsigned char _pad[0x18];
    CThingCreatureBase* PCreature;              // +0x18
};

struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();               // slot 0
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08();
    virtual void Cleanup();                     // slot 9 (+0x24)
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad[0x0c];                   // +0x08..+0x13
    CWorld* GetPWorld();                        // 0x008FCFC0
    CCombatManager* GetPCombatManager() { return GetPWorld()->m_pCombatManager; }
};

struct CAIStateGroup_MeleeCombatNearby2 : CAIStateGroupBase {
    virtual void Cleanup();
};

void CAIStateGroup_MeleeCombatNearby2::Cleanup()
{
    if (PBrain->PCreature->IsRegisteredCombatant()) {
        CThingCreatureBase* creature = PBrain->PCreature;
        GetPCombatManager()->DeregisterCombatant(creature);
    }
    if (PBrain->PCreature->UsesCreatureModes()) {
        CTCCreatureModeManager* modes;
        PBrain->PCreature->GetInterface(&modes, 0x31);
        modes->RemoveMode(0x19);
    }
}
