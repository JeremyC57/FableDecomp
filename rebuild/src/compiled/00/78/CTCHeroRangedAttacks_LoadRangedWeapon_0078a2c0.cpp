// CTCHeroRangedAttacks::LoadRangedWeapon() -- starts the hero's load action when the carried projectile weapon is alive and not yet loaded
// Leaf: ?LoadRangedWeapon@CTCHeroRangedAttacks
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0078A2C0, 207 bytes.
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020 (the hero's carrying component, type 0x46)
//          and its const twin @ 0x004365B0 (the weapon component, type 0x48), CTCCarrying::GetThingInPrimarySlot()
//          @ 0x006A1440, CTCWeapon::IsProjectileWeapon() @ 0x005DAA40, CTCWeapon::IsProjectileWeaponLoaded()
//          @ 0x005DA980, CCreatureAction_HeroLoadRangedWeapon::CCreatureAction_HeroLoadRangedWeapon(CThingCreatureBase*,
//          CThing*) @ 0x0078AE30 / its destructor @ 0x0078AB20 (label "_Dest_val<..CSymbolInfo..>" is propagated),
//          CThingCreatureBase::SetCurrentAction(const CCreatureActionBase&) @ 0x006644F0.
// Shape: the unchecked out-parameter GetInterface for the carrying component (raw bit test `test byte ptr
// [thing+28h],40h`, the absent arm reads the never-written key word); IS_THING_ALIVE inlined (null, dead bit);
// the weapon probe is the masked HasTC word (`test ah,1`) then the const lookup; the load action is a temporary
// built in the 0xB8-byte frame area and bound to SetCurrentAction's const reference straight from the ctor's eax.
// Layout: engine/CTCHeroRangedAttacks.h (PDB offsets match retail: PParentCreature +0x0C, Loading +0x17); the class is
// modelled locally because the header flattens CTCBase (vptr and ParentThing as pads).
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

class CCreatureActionBase { public:                         // retail 0xA8 bytes
    unsigned char _data[0xA8];
};
class CThingCreatureBase { public:
    bool SetCurrentAction(const CCreatureActionBase& action);   // 0x006644F0
};

class CCreatureAction_HeroLoadRangedWeapon : public CCreatureActionBase { public:   // 0xB8 bytes in retail
    unsigned char _data_0xa8[0x10];
    CCreatureAction_HeroLoadRangedWeapon(CThingCreatureBase* creature, CThing* weapon);   // 0x0078AE30
    ~CCreatureAction_HeroLoadRangedWeapon();         // 0x0078AB20
};

class CTCHeroRangedAttacks { public:
    void*               __vftable;                   // +0x00
    CThing*             ParentThing;                 // +0x04 CTCBase
    unsigned char       _pad_0x08[0x04];
    CThingCreatureBase* PParentCreature;             // +0x0C
    void*               PHeroCombatDef_Object;       // +0x10
    bool                FireCycleInProgress;         // +0x14
    bool                Aiming;                      // +0x15
    bool                FireQueued;                  // +0x16
    bool                Loading;                     // +0x17
    void LoadRangedWeapon();
};

void CTCHeroRangedAttacks::LoadRangedWeapon()
{
    CTCCarrying* carrying;
    ParentThing->GetInterface(&carrying, 0x46);
    CThing* weapon = carrying->GetThingInPrimarySlot();
    if (IS_THING_ALIVE(weapon) && weapon->HasTC(0x48)) {
        CTCWeapon* tc = weapon->FindTC<CTCWeapon>(0x48);
        if (tc->IsProjectileWeapon() && !tc->IsProjectileWeaponLoaded() && !Loading)
            PParentCreature->SetCurrentAction(CCreatureAction_HeroLoadRangedWeapon(PParentCreature, weapon));
    }
}