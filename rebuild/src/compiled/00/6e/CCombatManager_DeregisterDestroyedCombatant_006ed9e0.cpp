// CCombatManager::DeregisterDestroyedCombatant(CThingCreatureBase* creature) -- take a destroyed creature out of combat: as an attacker (telling its target's stealth component that the danger is over) and as an attacked combatant
// Retail: Fable.exe (TLC, VC7.1) @ 0x006ED9E0, 148 bytes (160 with padding).
// Callees: the creature's attacking test 0x00662410 and combatant test 0x00662430 (manifest labels
//          propagated), the combatant map's find @ 0x006F0250, CCombatManager::DeregisterAttackingCombatant
//          0x006ED510 / DeregisterAttackedCombatant 0x006ED8C0, CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound
//          0x0040F020, CTCStealth::SetAsInDanger(CThing*, EDangerStatus) 0x00747B80.
//
// Shape: the landed CCombatManager model (Combatants at +0x10, node value at +0x18, AttackerInfo.Attacked at
// value +4) and the landed GetInterface out-parameter template (CThingNoise::PassToNoiseComponent 0x008386E0):
// interface 0x36 (stealth) is bit 22 of the bitset word at +0x24, the interface map at +0x44 and the
// lookup key homed in the dead `creature` argument slot. The target is read from the combatant info
// BEFORE the attacker is deregistered (the node may go away); the 64-bit id is a local copied into the
// frame high word first. The template is `__forceinline`: as a plain inline member VC7.1 emitted it
// out of line in this TU (call + `push 0x36`).
struct CTCBase;

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);      // 0x0040F020
};

struct CThing {
    unsigned char _pad0[0x18];
    unsigned __int64 UniqueID;                  // +0x18
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
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

struct CThingCreatureBase : CThing {
    bool IsAttackingCombatant();                // 0x00662410
    bool IsCombatant();                         // 0x00662430
};

enum EDangerStatus { DANGER_NONE = 0 };

struct CTCStealth {                             // interface type 0x36
    void SetAsInDanger(CThing* source, EDangerStatus status);   // 0x00747B80
};

struct CCombatantInfo_Retail {
    CThingCreatureBase* Parent;                 // +0x00
    CThingCreatureBase* Attacked;               // +0x04 AttackerInfo.Attacked
};

struct SCombatantNode {                         // STLport _Rb_tree_node<pair<const u64, CCombatantInfo>>
    unsigned char    _links[0x10];              // colour, parent, left, right
    unsigned __int64 Key;                       // +0x10
    CCombatantInfo_Retail Value;                // +0x18
};

struct CCombatantMap {                          // STLport map<unsigned __int64, CCombatantInfo>
    SCombatantNode* FindNode(const unsigned __int64& key);   // 0x006F0250
};

struct CCombatManager {
    void*         __vftable;                    // +0x00
    unsigned char RegisteredCombatSequences[0xc];   // +0x04
    CCombatantMap Combatants;                   // +0x10
    void DeregisterAttackingCombatant(CThingCreatureBase* creature);   // 0x006ED510
    void DeregisterAttackedCombatant(CThingCreatureBase* creature);    // 0x006ED8C0
    void DeregisterDestroyedCombatant(CThingCreatureBase* creature);
};

void CCombatManager::DeregisterDestroyedCombatant(CThingCreatureBase* creature)
{
    if (creature->IsAttackingCombatant()) {
        unsigned __int64 id = creature->UniqueID;
        CThingCreatureBase* attacked = Combatants.FindNode(id)->Value.Attacked;
        DeregisterAttackingCombatant(creature);
        CTCStealth* stealth;
        if (attacked->GetInterface(&stealth, 0x36))
            stealth->SetAsInDanger(creature, DANGER_NONE);
    }
    if (creature->IsCombatant())
        DeregisterAttackedCombatant(creature);
}