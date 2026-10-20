// CTCFireRangedWeaponOnLeavingStateGroup::FrameUpdate() -- once: fire a loaded projectile weapon held in the primary carry slot, else let the combat manager hand the creature its next action; then remove this component
// Leaf: FrameUpdate
//
// Retail 0x007E5B90, 304 bytes incl. padding (vtable 0x0123E464 slot 9, the CTCBase FrameUpdate slot; catalog "vfunc_9").
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound const @ 0x004365B0 (the creature's CTCCarrying, type 0x46 =
// bit 6 of the bitset word +0x28) and the non-const copy @ 0x0040F020 (the carried thing's CTCWeapon, type 0x48 = bit 8,
// `test ch,1`), CTCCarrying::GetThingInPrimarySlot @ 0x006A1440, CTCWeapon::IsProjectileWeapon @ 0x005DAA40,
// CTCWeapon::IsProjectileWeaponLoaded @ 0x005DA980, CTCWeapon member @ 0x005DA4C0 (one store: +0x18 = 1; label
// "CXMVPlaybackInfo" is propagated, name descriptive), CThingCreatureBase::IsRegisteredCombatant @ 0x00662410 (twice),
// CTCBase::GetWorld @ 0x00686A70 (twice), CCombatManager::GetNextActionForCreature @ 0x006EB600,
// CThingCreatureBase::IsPerformingAnyAction @ 0x006622A0, CThingCreatureBase::SetCurrentAction @ 0x006644F0,
// CCombatManager::SetCreatureAsAttemptedToPerformedAction @ 0x006EA540, CThing::RemoveTC(long) @ 0x004C9840.
// Layout: Ego_r.pdb CTCFireRangedWeaponOnLeavingStateGroup (0x10): QuitOut +0xc; CWorld combat manager +0x4c; retail CThing
// bitset +0x20, interface map +0x44, dead bit +0x91.
// Shape: QuitOut is raised before the weapon test and dropped again when the weapon fires; the out-parameter GetInterface
// template (miss path reads the never-written slot); the creature half is a SECOND `if (!QuitOut)` block after the
// first (nested inside it, VC sinks the esi push below the entry); the creature's lookup goes through a const thing, so it reaches the
// const LowerBound; the world is taken into a local before the last call so it is fetched ahead of the pushes. The lookup key and the `performed` flag share the single `push ecx` frame word.
class CTCBase;
struct CCombatActionBase;

struct CInterfaceMap {                          // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44: {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                   // 0x0040F020
    const SEntry* LowerBound(const long* type) const;       // 0x004365B0
};

class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char Flags91;                      // +0x91 bit 0 Dead
    void RemoveTC(long interfaceType);          // 0x004C9840
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
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};

class CThingCreatureBase : public CThing {
public:
    bool IsRegisteredCombatant();               // 0x00662410
    bool IsPerformingAnyAction();               // 0x006622A0
    bool SetCurrentAction(CCombatActionBase* action);   // 0x006644F0
};

struct CTCCarrying {
    CThing* GetThingInPrimarySlot();            // 0x006A1440
};
struct CTCWeapon {
    bool IsProjectileWeapon();                  // 0x005DAA40
    bool IsProjectileWeaponLoaded();            // 0x005DA980
    void SetFireRequested();                    // 0x005DA4C0 (+0x18 = 1)
};

struct CCombatManager {
    CCombatActionBase* GetNextActionForCreature(CThingCreatureBase* creature);           // 0x006EB600
    void SetCreatureAsAttemptedToPerformedAction(CThingCreatureBase* creature, bool performed);   // 0x006EA540
};
struct CWorld {
    unsigned char _pad00[0x4c];
    CCombatManager* PCombatManager;             // +0x4c
    CCombatManager* GetPCombatManager() { return PCombatManager; }
};

class CTCBase {
public:
    virtual void v00();
    CThing* ParentThing;                        // +0x04
    bool    ToBeRemoved;                        // +0x08
    bool    IsInGlobalUpdateTCs;                // +0x09
    CWorld* GetWorld();                         // 0x00686A70
};

class CTCFireRangedWeaponOnLeavingStateGroup : public CTCBase {
public:
    bool QuitOut;                               // +0x0c
    void FrameUpdate();
};

void CTCFireRangedWeaponOnLeavingStateGroup::FrameUpdate()
{
    if (!QuitOut) {
        const CThing* thing = ParentThing;
        QuitOut = true;
        CTCCarrying* carrying;
        if (thing->GetInterface(&carrying, 0x46)) {
            CThing* item = carrying->GetThingInPrimarySlot();
            CTCWeapon* weapon;
            if (item && !(item->Flags91 & 1) && item->GetInterface(&weapon, 0x48)) {
                if (weapon->IsProjectileWeapon() && weapon->IsProjectileWeaponLoaded()) {
                    weapon->SetFireRequested();
                    QuitOut = false;
                }
            }
        }
    }
    if (!QuitOut) {
        CThingCreatureBase* creature = (CThingCreatureBase*)ParentThing;
        if (creature->IsRegisteredCombatant()) {
            CCombatActionBase* action = GetWorld()->GetPCombatManager()->GetNextActionForCreature(creature);
            if (creature->IsRegisteredCombatant()) {
                bool performed = false;
                if (action && !creature->IsPerformingAnyAction() && creature->SetCurrentAction(action))
                    performed = true;
                CWorld* world = GetWorld();
                world->PCombatManager->SetCreatureAsAttemptedToPerformedAction(creature, performed);
            }
        }
    }
    ParentThing->RemoveTC(0x104);
}
