// CCombatManager::SetAttackerAsReadyToAttack(const CThingCreatureBase& creature, bool keepSequence) -- for an attacking combatant, reload a carried projectile weapon (alive item with a CTCWeapon that is a projectile weapon), then reset its sequence unless told to keep it
// Leaf: SetAttackerAsReadyToAttack
// Retail: Fable.exe (TLC, VC7.1) @ 0x006EA450, 238 bytes (240 with padding), `ret 8`.
// Callees: the combatant map's find 0x006F0250 (twice), CVectorMap::LowerBound 0x0040F020 (inlined GetInterface for
//   type 0x46 = CTCCarrying, bit 6 of +0x28, and 0x48 = CTCWeapon, bit 8 of +0x28), CTCCarrying::GetThingInPrimarySlot
//   0x006A1440, CTCWeapon::IsProjectileWeapon 0x005DAA40, the weapon call 0x005DA4E0 (manifest "CGhostGrannySon::Init" is
//   propagated), CCombatManager::PerformSequenceReset 0x006EA2C0.
// Layout: CCombatManager / UID map as the landed 0x006E9E40 (Combatants +0x10; value Parent +0, AttackerInfo.Attacked +4);
//   the thing's interface bitset +0x20 / vector map +0x44 / dead bit +0x91 as the landed 0x0095D310.
// Shape: the UID is read once into a named local and handed by value to the inline IsCreatureAttacking / GetCombatantInfo
//   (landed 0x006E9E40); the interface key temporaries live in the dead `creature` slot.
struct CTCBase;

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};

struct CThing {
    unsigned char _pad0[0x18];
    unsigned __int64 UniqueID;                  // +0x18
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x45];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    unsigned __int64 GetUID() const { return UniqueID; }
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

struct CThingCreatureBase : CThing { };

struct CTCCarrying {                            // interface type 0x46
    CThing* GetThingInPrimarySlot();            // 0x006A1440
};

struct CTCWeapon {                              // interface type 0x48
    bool IsProjectileWeapon();                  // 0x005DAA40
    void ReadyProjectile();                     // 0x005DA4E0
};

struct CCombatantInfo {                         // retail layout (Ego_r.pdb names)
    CThingCreatureBase* Parent;                 // +0x00
    CThingCreatureBase* Attacked;               // +0x04 AttackerInfo.Attacked
    unsigned char _rest[0x70 - 0x08];
};

struct CRetailRbMap_UID_CombatantInfo {         // STLport map<unsigned __int64, CCombatantInfo>
    struct node {
        long  colour; node* parent; node* left; node* right;
        unsigned __int64 key;                   // +0x10
        CCombatantInfo value;                   // +0x18
    };
    node* header;
    unsigned long count;
    unsigned long comp;
    node* find(const unsigned __int64& key);    // 0x006F0250
    node* end() { return header; }
};

struct CCombatManager {
    virtual void v00();
    unsigned char _sequences[0xc];              // +0x04
    CRetailRbMap_UID_CombatantInfo Combatants;  // +0x10
    CCombatantInfo& GetCombatantInfo(unsigned __int64 uid)
    {
        return Combatants.find(uid)->value;
    }
    bool IsCreatureAttacking(unsigned __int64 uid)
    {
        CRetailRbMap_UID_CombatantInfo::node* it = Combatants.find(uid);
        return it != Combatants.end() && it->value.Attacked != 0;
    }
    void PerformSequenceReset(CThingCreatureBase* creature);   // 0x006EA2C0
    void SetAttackerAsReadyToAttack(const CThingCreatureBase& creature, bool keepSequence);
};

void CCombatManager::SetAttackerAsReadyToAttack(const CThingCreatureBase& creature, bool keepSequence)
{
    unsigned __int64 uid = creature.GetUID();
    if (!IsCreatureAttacking(uid))
        return;
    CThingCreatureBase* attacker = GetCombatantInfo(uid).Parent;
    CTCCarrying* carrying;
    if (attacker->GetInterface(&carrying, 0x46)) {
        CThing* item = carrying->GetThingInPrimarySlot();
        CTCWeapon* weapon;
        if (item && !(item->m_flags91 & 1) && item->GetInterface(&weapon, 0x48) && weapon->IsProjectileWeapon())
            weapon->ReadyProjectile();
    }
    if (!keepSequence)
        PerformSequenceReset(attacker);
}
