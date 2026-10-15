// CAIStateGroup_MeleeCombatNearbyAttackWhenVeryClose::Cleanup() -- deregister the creature from its combat manager if it is a registered combatant, then drop creature mode 0x19
// Leaf: ?Cleanup@CAIStateGroup_MeleeCombatNearbyAttackWhenVeryClose@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0093E560, length 121 bytes (+ int3 padding).
// Callees: CThingCreatureBase::IsRegisteredCombatant @ 0x00662410 (landed name),
// CThingCreatureBase::GetCombatManager @ 0x00661ED0, CCombatManager::DeregisterCombatant(CThingCreatureBase*)
// @ 0x006ED9C0, CVectorMap::LowerBound @ 0x0040F020 (inlined interface lookup, type 0x31 =
// CTCCreatureModeManager), CTCCreatureModeManager::RemoveMode(unsigned long) @ 0x0073ABA0.
//
// Shape: `creature->GetCombatManager()->DeregisterCombatant(creature)` with the creature re-read once into
// ecx, pushed as the argument first and then used as `this` of the getter (arguments before the object
// expression); then the out-parameter GetInterface template with its result ignored, RemoveMode(0x19)
// tail-duplicated into the hit and miss arms (the miss arm reads the never-written manager local from the
// key's `push ecx` slot). No engine header models the state group's vptr chain: local model.
class CTCBase;

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);      // 0x0040F020
};

struct CCombatManager;

struct CThingCreatureBase {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    bool IsRegisteredCombatant();               // 0x00662410
    CCombatManager* GetCombatManager();         // 0x00661ED0
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
};

struct CAIStateGroup_MeleeCombatNearbyAttackWhenVeryClose : CAIStateGroupBase {
    virtual void Cleanup();
};

void CAIStateGroup_MeleeCombatNearbyAttackWhenVeryClose::Cleanup()
{
    if (PBrain->PCreature->IsRegisteredCombatant())
        PBrain->PCreature->GetCombatManager()->DeregisterCombatant(PBrain->PCreature);
    CTCCreatureModeManager* modes;
    PBrain->PCreature->GetInterface(&modes, 0x31);
    modes->RemoveMode(0x19);
}