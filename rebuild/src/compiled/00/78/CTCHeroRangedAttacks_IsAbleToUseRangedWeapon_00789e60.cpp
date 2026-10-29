// CTCHeroRangedAttacks::IsAbleToUseRangedWeapon() -- the hero may fire a ranged weapon: no blocking interface-167 state, the player is not locked out, the current action is not a long type-100 action, the primary-slot item is a live projectile weapon and the current mode is 25 or 70
// Leaf: IsAbleToUseRangedWeapon
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00789E60, 363 bytes (+ int3 padding), `ret` (thiscall, no arguments).
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound const @ 0x004365B0 (the inlined interface lookups,
//   types 0xA7 = word 5 bit 7 (`test al,al ; jns` on +0x34), 0x46 CTCCarrying = word 2 bit 6, 0x48 CTCWeapon = word 2
//   bit 8, 0x31 = word 1 bit 17), CThing::PeekPlayer @ 0x004C7A10, CTCCarrying::GetThingInPrimarySlot @ 0x006A1440,
//   CTCWeapon::IsProjectileWeapon @ 0x005DAA40, the interface-49 component's current-mode getter @ 0x0073A6E0
//   (manifest "CPixelShader::CountReferences" is propagated), the mode's virtual slot 1 (its type id).
// Shape: the first lookup and GetPlayer share the owner thing in edi (a block-scoped local); every later test re-reads
//   ParentThing. The carrying and mode lookups are the out-parameter GetInterface with the bool ignored (absent path
//   reads the uninitialised out word, which shares the `push ecx` key slot); the weapon lookup sits behind its own
//   bit test in the item `&&` chain. All failures share one `xor al,al` exit.
// Layout: this from engine/CTCHeroRangedAttacks.h (ParentThing +0x04 is the flattened CTCBase owner, PParentCreature
//   +0x0c); retail CThing interface bitset +0x20, vector map +0x44, dead bit +0x91 (landed relatives 0x006EA450,
//   0x007B0880); CThingCreatureBase +0x100 is the current action's data pointer in retail (PDB +0x11c, STLport
//   containers 4 bytes shorter); CPlayer +0x218 is a control-lock byte.
#include "engine/CTCHeroRangedAttacks.h"

class CTCBase;

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                              // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const ETCInterfaceType& type) const;     // 0x004365B0
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CPlayer { public:
    unsigned char _pad000[0x218];
    bool m_controlsLocked;                          // +0x218
};

class CThing { public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                 // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                     // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                        // +0x91 bit 0: dead
    __forceinline bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    __forceinline CTCBase* GetTC(long type) const
    {
        ETCInterfaceType key = (ETCInterfaceType)(type);
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(key);
        CInterfaceMap::SEntry* end = map->last;
        return (it == end || type < it->first ? end : it)->second;
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->first)
                it = map->last;
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
public:
    const CPlayer& PeekPlayer() const;   // 0x004C7A10
};

class CCreatureActionBase { public:
    unsigned char _pad00[0x20];
    long m_type;                                    // +0x20
    long m_elapsed;                                 // +0x24
};

class CThingCreatureBase { public:
    unsigned char _pad000[0x100];
    CCreatureActionBase* PCurrentAction;            // +0x100 (retail) CCountedPointer<CCreatureActionBase>::Data
};

struct CTCRangedBlocker {                           // interface 0xA7
    unsigned char _pad00[0xc];
    bool m_blocking;                                // +0x0c
};

class CTCCarrying { public:                                // interface 0x46
public:
    CThing* GetThingInPrimarySlot() const;   // 0x006A1440
};

class CTCWeapon { public:                                  // interface 0x48
public:
    bool IsProjectileWeapon() const;   // 0x005DAA40
};

struct CMode {
    virtual void v00();
    virtual long GetType();                         // slot 1
};

struct CTCModeOwner {                               // interface 0x31
    CMode* GetCurrentMode();                        // 0x0073A6E0
};

struct CTCHeroRangedAttacksMethods : CTCHeroRangedAttacks {
    CThing* GetOwner() const { return *(CThing* const*)_pad_0x04; }   // CTCBase::ParentThing (+0x04)
    bool IsAbleToUseRangedWeapon();
};

bool CTCHeroRangedAttacksMethods::IsAbleToUseRangedWeapon()
{
    {
        CThing* thing = GetOwner();
        CTCRangedBlocker* blocker;
        if (thing->GetInterface(&blocker, 0xa7) && blocker->m_blocking)
            return false;
        if (thing->PeekPlayer().m_controlsLocked)
            return false;
    }
    CCreatureActionBase* action = ((CThingCreatureBase*)PParentCreature)->PCurrentAction;
    if (action && action->m_type == 100 && action->m_elapsed >= 250)
        return false;

    CTCCarrying* carrying;
    GetOwner()->GetInterface(&carrying, 0x46);
    CThing* item = carrying->GetThingInPrimarySlot();
    CTCWeapon* weapon;
    if (!item || (item->m_flags91 & 1) || !item->GetInterface(&weapon, 0x48) || !weapon->IsProjectileWeapon())
        return false;

    CTCModeOwner* modeOwner;
    GetOwner()->GetInterface(&modeOwner, 0x31);
    long type = modeOwner->GetCurrentMode()->GetType();
    if (type == 25 || type == 70)
        return true;
    return false;
}