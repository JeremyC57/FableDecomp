// NHeroExperience::GetWeaponModifiedExperienceMultiplierForThing(const CThing& thing) -- the experience multiplier of the thing's active weapons: 1.0, replaced by a live augmented melee weapon's multiplier and then scaled by a live augmented ranged weapon's
// Leaf: ?GetWeaponModifiedExperienceMultiplierForThing@NHeroExperience@@YIMABVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006D76C0, 256 bytes, `ret` (__fastcall: the thing in ecx), x87 result.
// Identity: the catalog label is the FableWin.pdb name ?GetWeaponModifiedExperienceMultiplierForThing@NHeroExperience@@
//   YIMABVCThing@@@Z (namespace function). Callers 0x006D8333 and 0x0077A8C4.
// Callees: the interface map's const LowerBound 0x004365B0 (the inventory-weapons component, TC 0x13, and each weapon's
//   object augmentations, TC 0xBF), CBaseIntelligentPointer::GetPItem 0x00A01B50 (both active weapons),
//   CTCObjectAugmentations::GetExperienceMultiplier 0x00766400 (landed).
// Layout: retail CThing interface bitset +0x20 (TC 0x13 = bit 19 of word 0, TC 0xBF = bit 31 of word 5 at +0x34), the
//   interface map +0x44 ({first, last} of (type, component) pairs), Dead = bit 0 of +0x91; CTCInventoryWeapons
//   PActiveMeleeWeapon +0x134 / PActiveRangedWeapon +0x141 (unaligned, the landed 0x005C30D3 model).
// Shape: the multiplier a float local initialised to 1.0 (`mov [esp],3F800000h`); both weapon pointers fetched first; the
//   melee weapon's multiplier assigned, the ranged one multiplied in (`fmul [local]`), each guarded by alive + HasTC.
class CTCBase;
class CTCObjectAugmentations;

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const long* type) const;       // 0x004365B0
};

class CThing {
public:
    virtual ~CThing();
    unsigned char _pad04[0x20 - 0x04];
    unsigned long UsingComponents[9];                       // +0x20
    CInterfaceMap Components;                               // +0x44
    unsigned char _pad4c[0x91 - 0x4C];
    unsigned char StateBits;                                // +0x91 Dead 0x01
    bool IsDead() const { return (StateBits & 0x01) != 0; }
    __forceinline unsigned long HasTC(long type) const { return UsingComponents[type >> 5] & (1UL << (type & 31)); }
    template <class T> __forceinline const T* GetConstTC(long interfaceType) const
    {
        long type = interfaceType;
        const CInterfaceMap* map = &Components;
        const CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return (const T*)it->component;
    }
};

struct CIntelligentPointer {
    unsigned char _body[8];
    CThing* GetPItem() const;                               // 0x00A01B50
};

class CTCInventoryWeapons {
public:
    unsigned char _pad00[0x134];
    CIntelligentPointer PActiveMeleeWeapon;                 // +0x134
    long ActiveMeleeWeaponDefIndex;                         // +0x13C
    bool MeleeWeaponCarriedOnLoad;                          // +0x140
    CIntelligentPointer PActiveRangedWeapon;                // +0x141
};

class CTCObjectAugmentations {
public:
    float GetExperienceMultiplier() const;                  // 0x00766400
};

enum {
    TC_INVENTORY_WEAPONS = 0x13,
    TC_OBJECT_AUGMENTATIONS = 0xBF
};

namespace NHeroExperience {

float __fastcall GetWeaponModifiedExperienceMultiplierForThing(const CThing& thing)
{
    float multiplier = 1.0f;
    if (thing.HasTC(TC_INVENTORY_WEAPONS)) {
        const CTCInventoryWeapons* weapons = thing.GetConstTC<CTCInventoryWeapons>(TC_INVENTORY_WEAPONS);
        const CThing* melee = weapons->PActiveMeleeWeapon.GetPItem();
        const CThing* ranged = weapons->PActiveRangedWeapon.GetPItem();
        if (melee && !melee->IsDead() && melee->HasTC(TC_OBJECT_AUGMENTATIONS))
            multiplier = melee->GetConstTC<CTCObjectAugmentations>(TC_OBJECT_AUGMENTATIONS)->GetExperienceMultiplier();
        if (ranged && !ranged->IsDead() && ranged->HasTC(TC_OBJECT_AUGMENTATIONS))
            multiplier *= ranged->GetConstTC<CTCObjectAugmentations>(TC_OBJECT_AUGMENTATIONS)->GetExperienceMultiplier();
    }
    return multiplier;
}

}