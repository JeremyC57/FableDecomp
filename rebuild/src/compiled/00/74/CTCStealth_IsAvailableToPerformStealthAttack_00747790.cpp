// CTCStealth::IsAvailableToPerformStealthAttack(CThing* target) -- true when the owner carries a live weapon in its primary slot and that weapon can stealth-attack the target
// Leaf: IsAvailableToPerformStealthAttack
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: map pointer made const (the row grades the const LowerBound) re-padded by-value maps +8.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00747790, length 141 bytes (`ret 4`).
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x004365B0 (two inlined GetTC lookups: 0x46 CTCCarrying = bit 6 of
// CThing+0x28, 0x48 CTCWeapon = bit 8), CTCCarrying::GetThingInPrimarySlot @ 0x006A1440, CTCWeapon::IsCapableOfStealthAttackBy(CThing*)
// @ 0x005DAD60.
// Layout: CThing interface bitset +0x20, interface map +0x44, dead flag +0x91 bit 0.
// Shape: both components come through the out-parameter GetInterface template: the two lookups then share one key frame word and the
// first bit test stays `test byte ptr [eax+28h],40h` (HasTC + GetTC gave two key slots and `mov cl,[eax+28h]`); the weapon's result is
// returned directly.
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad54[0x3d];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    __forceinline bool HasTC(long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)))
            return true;
        return false;
    }
    __forceinline CTCBase* GetTC(long interfaceType)
    {
        ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::value_type* it = map->LowerBound(type);
        if (it == map->end() || interfaceType < it->first)
            it = map->end();
        return it->second;
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::value_type* it = map->LowerBound(type);
            if (it == map->end() || interfaceType < it->first)
                it = map->end();
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
};
class CTCCarrying { public:
public:
    CThing* GetThingInPrimarySlot() const;   // 0x006A1440
};
class CTCWeapon { public:
    bool IsCapableOfStealthAttackBy(CThing* thing);   // 0x005DAD60
};
class CTCStealth { public:
    virtual void v00();
    CThing* ParentThing;                        // +0x04 (CTCBase)
    bool IsAvailableToPerformStealthAttack(CThing* target);
};

bool CTCStealth::IsAvailableToPerformStealthAttack(CThing* target)
{
    CTCCarrying* carrying;
    if (ParentThing->GetInterface(&carrying, 0x46)) {
        CThing* weapon = carrying->GetThingInPrimarySlot();
        if (weapon && !(weapon->m_flags91 & 1)) {
            CTCWeapon* tc;
            if (weapon->GetInterface(&tc, 0x48))
                return tc->IsCapableOfStealthAttackBy(target);
        }
    }
    return false;
}