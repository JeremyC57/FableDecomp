// CTCSpecialAbilities::CalculateWeaponState(CThingCreatureBase& creature) -- which weapon pose a spell cast uses: unarmed with nothing (alive) in the primary carrying slot, armed when the held thing is not the active ranged weapon, else the bow pose unless the ranged weapon's type is 4 (armed)
// Leaf: ?CalculateWeaponState@CTCSpecialAbilities@@SI?AW4EWEAPON_STATE@1@AAVCThingCreatureBase@@@Z
// Plan phase 4 (2026-11-05), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: the local map's CArray member Data is the header's CArray base (Ego_r: CVectorMap derives CArray<pair<K,V> >): Data.end() / begin() / size() read through the base; the local std::bitset operator[] / reference is the STLport model of stlport/fable_containers.h under FABLE_RETAIL_STLP_BITSET_INDEX (retail tests the word inline, e.g. 0x0062D032 test dword ptr [ebx+0x24],0x10000); the local same-TU LowerBound bodies removed (the header defines the same-TU noinline bodies); the empty local std::bitset stand-in is the STLport bitset<274> (36 bytes) of stlport/fable_containers.h, test() declared only under FABLE_RETAIL_STLP_BITSET_TEST_DECL (retail calls 0x00410DE0).
// Retail: Fable.exe (TLC, VC7.1) @ 0x0072B940, 247 bytes, static __fastcall (ecx = creature), plain `ret`.
// Identity (proven): Ego_r.pdb / FableWin.pdb ?CalculateWeaponState@CTCSpecialAbilities@@SI?AW4EWEAPON_STATE@1@AAVCThingCreatureBase@@@Z
//   (Ego_r donor 0x00742E70, a static __fastcall member of CTCSpecialAbilities returning the nested EWEAPON_STATE); the landed
//   CTCWillLightning::SetWeaponState 0x0079F680 / CTCFireballSpell::SetWeaponState 0x007C8660 take the same enum, and their
//   anim-name switches pick the left/right-handed (0), "ARMED" (1) and "WITH_BOW" (2) animations.
// Callees: CVectorMap<ETCInterfaceType,CTCBase*,..>::LowerBound const 0x004365B0 / non-const 0x0040F020, CTCCarrying::
//   GetThingInPrimarySlot() const 0x006A1440, CBaseIntelligentPointer::GetPItem() const 0x00A01B50, CTCWeapon::GetWeaponType()
//   const 0x005DA7D0.
// Layout: retail CThing UsingComponents +0x20 / Components +0x44 / Dead +0x91 bit 0; CTCInventoryWeapons (interface 0x13)
//   PActiveRangedWeapon +0x141 (Ego_r +0x159 less the 0x18 the PC STLport containers save, as PActiveMeleeWeapon +0x134 of the
//   landed 0x007CCD70); interfaces 0x46 CTCCarrying, 0x48 CTCWeapon.
// Shape: the inventory pointer is an uninitialised local filled only under HasTC(0x13) (the untaken branch reads the stale key
//   slot, GOTCHAS chunk 264); the creature's lookups are const (PeekTC), the weapon's is the non-const GetTC; both are
//   __forceinline (VC7.1 otherwise keeps the second and third out of line) over same-TU noinline LowerBound bodies (chunk 242).
#include "fable_compiler.h"
class CBaseObject;
class CBaseObjectPointer;
class CTCBase;
enum ETCInterfaceType { ETCInterfaceType_Force32 = 0x7fffffff };
enum EWeaponType { EWeaponType_Force32 = 0x7fffffff };   // value 4 is the one ranged type posed as armed (descriptive)

#define FABLE_RETAIL_STLP_BITSET_INDEX
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#include "engine/CVectorMap.h"

// Same-TU bodies of the two real retail LowerBound instances (binary search over the sorted pairs), not inlined: they let the
// three inlined lookups share one key frame word with the uninitialised inventory local, as retail does.
#pragma auto_inline(off)
#pragma auto_inline(on)

class CThing {                                          // partial view, retail offsets
public:
    typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > TCMap;
    unsigned char      Header[0x20];
    std::bitset<274>   UsingComponents;                 // +0x20
    TCMap              Components;                      // +0x44
    unsigned char      _pad54[0x91 - 0x54];
    unsigned char      Dead : 1;                        // +0x91 bit 0
    bool HasTC(ETCInterfaceType type) const { return UsingComponents[type]; }
    template <class T> FABLE_FORCEINLINE const T* PeekTC(ETCInterfaceType type) const
    {
        ETCInterfaceType key = type;
        const TCMap* map = &Components;
        const TCMap::value_type* it = map->LowerBound(key);
        return (const T*)((it == map->end() || type < it->first) ? map->end() : it)->second;
    }
    template <class T> FABLE_FORCEINLINE T* GetTC(ETCInterfaceType type)
    {
        ETCInterfaceType key = type;
        TCMap* map = &Components;
        TCMap::value_type* it = map->LowerBound(key);
        return (T*)((it == map->end() || type < it->first) ? map->end() : it)->second;
    }
};
class CThingCreatureBase : public CThing { };

class CBaseIntelligentPointer {
public:
    CBaseIntelligentPointer();                          // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
    CBaseObject* GetPItem() const;                      // 0x00A01B50
    CBaseObjectPointer* PData;                          // +4
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // vtable 0x01238C6C
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};

class CTCCarrying {
public:
    CThing* GetThingInPrimarySlot() const;              // 0x006A1440
};
class CTCWeapon {
public:
    EWeaponType GetWeaponType() const;                  // 0x005DA7D0
};
#pragma pack(push, 1)
class CTCInventoryWeapons {                             // interface 0x13, partial view (retail offsets)
public:
    unsigned char               _pad000[0x141];
    CIntelligentPointer<CThing> PActiveRangedWeapon;    // +0x141
};
#pragma pack(pop)

class CTCSpecialAbilities {
public:
    enum EWEAPON_STATE {                                // enumerator names descriptive (from the landed anim-name switches)
        WEAPON_STATE_UNARMED = 0,
        WEAPON_STATE_ARMED   = 1,
        WEAPON_STATE_BOW     = 2
    };
    static EWEAPON_STATE __fastcall CalculateWeaponState(CThingCreatureBase& creature);
};

CTCSpecialAbilities::EWEAPON_STATE __fastcall CTCSpecialAbilities::CalculateWeaponState(CThingCreatureBase& creature)
{
    if (creature.HasTC((ETCInterfaceType)0x46)) {
        CThing* weapon = creature.PeekTC<CTCCarrying>((ETCInterfaceType)0x46)->GetThingInPrimarySlot();
        if (weapon && !weapon->Dead) {
            const CTCInventoryWeapons* inventory;
            if (creature.HasTC((ETCInterfaceType)0x13))
                inventory = creature.PeekTC<CTCInventoryWeapons>((ETCInterfaceType)0x13);
            if (weapon == inventory->PActiveRangedWeapon.GetPItem()) {
                if (weapon->HasTC((ETCInterfaceType)0x48))
                    return weapon->GetTC<CTCWeapon>((ETCInterfaceType)0x48)->GetWeaponType() == (EWeaponType)4
                               ? WEAPON_STATE_ARMED : WEAPON_STATE_BOW;
                return WEAPON_STATE_BOW;
            }
            return WEAPON_STATE_ARMED;
        }
    }
    return WEAPON_STATE_UNARMED;
}