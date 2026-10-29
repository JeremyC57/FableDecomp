// CTCWeapon::IsUsableBy(const CThing& thing) const -- only creatures may use a weapon; class-1 things, a suppressed game, things without hero experience and weapon classes 0/1/3 always may; class 2 needs berserk or the experience requirements
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005DAB10, 179 bytes, `ret 4`, then a 4-entry jump table at 0x005DABC4.
// Ego_r.pdb `?IsUsableBy@CTCWeapon@@QBE_NABVCThing@@@Z`.
// Callees: CTCBase::GetGameSubsystem() @ 0x00686D60 (`[0x013B8A1C]+0x1c`, unidentified; its virtual slot 9 (+0x24)
//          IsSuppressed() returns a bool -- landed spelling of 0x007B6A80 / 0x007E1F70); the creature getter @ 0x006A98D0
//          (returns the word at +0x1e8, object unidentified; manifest "CWorld::DrawGetEnvironment" is propagated);
//          CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound const @ 0x004365B0 (interface 0x68 = CTCHeroExperience, bit 8
//          of the bitset dword at +0x2c); CTCWeapon::GetWeaponClass() const @ 0x005DA7F0 (manifest
//          "CCategory::GetCategoryIdentifier" is a byte-twin label); the requirement check @ 0x00851B50 on the getter
//          result's +0x13c sub-object with the hero-experience component (labelled "CAbility_CheckRequirements").
// Layout: CThing class byte +0x10, interface bitset +0x20 (CTCBerserk 0x84 = bit 4 of +0x30), components +0x44,
//          ThingTypeFlags +0x6c (bit 3: creature); CTCWeapon from engine/CTCWeapon.h (only GetWeaponClass is used).
// Shape: the creature test wraps the body and its single `return false` is shared with the switch's missing default
//          (that shared tail keeps all four pushes in the prologue; separate `return false`s sink ebp/esi past the
//          first exits); the getter is called before the interface test; the out-parameter GetInterface (its type
//          temporary reuses the dead argument slot); HasTC is __forceinline (plain inline is not expanded inside the
//          `||`); the class-2 arm returns an `||` (`mov eax,1` / `xor eax,eax` tails) while the other trues share `mov al,1`.
#include "engine/CTCWeapon.h"

class CTCBase;

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                          // CVectorMap<ETCInterfaceType, CTCBase*>
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const ETCInterfaceType& type) const;   // 0x004365B0 (the const instantiation)
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    virtual void v00();
    unsigned char _pad04[0x10 - 0x04];
    unsigned char ThingClass;                   // +0x10
    unsigned char _pad11[0x20 - 0x11];
    unsigned long m_presentBits[9];             // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x6c - 0x4c];
    unsigned long ThingTypeFlags;               // +0x6c bit 3: creature
    __forceinline unsigned long HasTC(long interfaceType) const { return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)); }
    template<class T> __forceinline bool GetInterface(const T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->first)
                it = map->last;
            *ppOut = (const T*)it->second;
            return true;
        }
        return false;
    }
};

class CTCHeroExperience;                       // interface 0x68
struct CExperienceRequirements {                // the +0x13c sub-object of the creature's +0x1e8 record
    bool IsMetBy(const CTCHeroExperience* experience) const;   // 0x00851B50
};
struct CCreatureRecord1E8 {                     // unidentified
    unsigned char           _pad00[0x13c];
    CExperienceRequirements Requirements;       // +0x13c
};
class CThingCreatureBase : public CThing { public:
    const CCreatureRecord1E8* GetRecord1E8() const;   // 0x006A98D0
};

struct CGameSubsystem {                         // [0x013B8A1C]+0x1c (unidentified)
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual bool IsSuppressed();                // slot 9 (+0x24)
};

enum EWeaponClass { WEAPON_CLASS_NONE = 0, WEAPON_CLASS_1 = 1, WEAPON_CLASS_2 = 2, WEAPON_CLASS_3 = 3 };

struct CTCWeapon_Getters : CTCWeapon {
    CGameSubsystem* GetGameSubsystem();         // 0x00686D60 (CTCBase)
    EWeaponClass GetWeaponClass() const;        // 0x005DA7F0
    bool IsUsableBy(const CThing& thing) const;
};

bool CTCWeapon_Getters::IsUsableBy(const CThing& thing) const
{
    if (thing.ThingTypeFlags & 8) {
        if (thing.ThingClass == 1 || ((CTCWeapon_Getters*)this)->GetGameSubsystem()->IsSuppressed())
            return true;
        const CCreatureRecord1E8* record = static_cast<const CThingCreatureBase&>(thing).GetRecord1E8();
        const CTCHeroExperience* experience;
        if (!thing.GetInterface(&experience, 0x68))
            return true;
        switch (GetWeaponClass()) {
        case WEAPON_CLASS_NONE:
        case WEAPON_CLASS_1:
        case WEAPON_CLASS_3:
            return true;
        case WEAPON_CLASS_2:
            return thing.HasTC(0x84) || record->Requirements.IsMetBy(experience);
        }
    }
    return false;
}