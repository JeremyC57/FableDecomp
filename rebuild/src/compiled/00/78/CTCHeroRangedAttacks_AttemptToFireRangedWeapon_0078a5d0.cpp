// CTCHeroRangedAttacks::AttemptToFireRangedWeapon() -- queues a shot from the carried projectile weapon and fires at once if it is loaded
// Leaf: AttemptToFireRangedWeapon
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0078A5D0, 178 bytes.
// Callees: CTCHeroRangedAttacks::IsAbleToUseRangedWeapon() @ 0x00789E60, CVectorMap<ETCInterfaceType,CTCBase*>::
//          LowerBound @ 0x0040F020 (carrying, type 0x46) and its const twin @ 0x004365B0 (weapon, type 0x48),
//          CTCCarrying::GetThingInPrimarySlot() @ 0x006A1440, CTCWeapon::IsProjectileWeapon() @ 0x005DAA40,
//          CTCWeapon::IsProjectileWeaponLoaded() @ 0x005DA980, CTCHeroRangedAttacks::FireRangedWeapon() @ 0x0078A390.
// Shape: the LoadRangedWeapon (0x0078A2C0) prelude: unchecked out-parameter GetInterface for the carrying component,
// IS_THING_ALIVE inlined, the masked HasTC word (`test ch,1`) and the const weapon lookup; FireQueued is set before
// the loaded test, the fire is the last call.
// Layout: engine/CTCHeroRangedAttacks.h offsets (FireQueued +0x16); local model because the header flattens CTCBase.
class CTCBase;

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                               // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);            // 0x0040F020
    const SEntry* LowerBound(const ETCInterfaceType& type) const;   // 0x004365B0
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                  // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                      // +0x44
    unsigned char _pad1[0x45];
    unsigned char m_flags91;                         // +0x91 bit 0: dead
    unsigned long HasTC(long interfaceType) const    // the masked word
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->first)
                it = map->last;
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
    template<class T> __forceinline T* FindTC(long interfaceType) const
    {
        ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(type);
        if (it == map->last || interfaceType < it->first)
            it = map->last;
        return (T*)it->second;
    }
};

inline bool IS_THING_ALIVE(const CThing* thing)      // pointer set and the dead bit (+0x91 bit 0) clear
{
    if (thing != 0) {
        if (!(thing->m_flags91 & 1))
            return true;
    }
    return false;
}

class CTCCarrying { public:                                 // interface type 0x46
public:
    CThing* GetThingInPrimarySlot() const;   // 0x006A1440
};

class CTCWeapon { public:                                   // interface type 0x48
    bool IsProjectileWeaponLoaded();                 // 0x005DA980
public:
    bool IsProjectileWeapon() const;   // 0x005DAA40
};

class CTCHeroRangedAttacks { public:
    void*         __vftable;                         // +0x00
    CThing*       ParentThing;                       // +0x04 CTCBase
    unsigned char _pad_0x08[0x04];
    void*         PParentCreature;                   // +0x0C CThingCreatureBase*
    void*         PHeroCombatDef_Object;             // +0x10
    bool          FireCycleInProgress;               // +0x14
    bool          Aiming;                            // +0x15
    bool          FireQueued;                        // +0x16
    bool          Loading;                           // +0x17
    bool IsAbleToUseRangedWeapon();                  // 0x00789E60
    void FireRangedWeapon();                         // 0x0078A390
    void AttemptToFireRangedWeapon();
};

void CTCHeroRangedAttacks::AttemptToFireRangedWeapon()
{
    if (IsAbleToUseRangedWeapon()) {
        CTCCarrying* carrying;
        ParentThing->GetInterface(&carrying, 0x46);
        CThing* weapon = carrying->GetThingInPrimarySlot();
        if (IS_THING_ALIVE(weapon) && weapon->HasTC(0x48)) {
            CTCWeapon* tc = weapon->FindTC<CTCWeapon>(0x48);
            if (tc->IsProjectileWeapon()) {
                FireQueued = true;
                if (tc->IsProjectileWeaponLoaded())
                    FireRangedWeapon();
            }
        }
    }
}