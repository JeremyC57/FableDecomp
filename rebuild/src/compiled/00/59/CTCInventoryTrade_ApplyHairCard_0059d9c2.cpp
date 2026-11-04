// CTCInventoryTrade::ApplyHairCard(const CHairCardDef*) -- if the owning thing has the attachable-appearance-modifiers component (interface 0x5E), applies the card's appearance modifier with replacement
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: the empty local std::bitset stand-in is the STLport bitset<274> (36 bytes) of stlport/fable_containers.h, test() declared only under FABLE_RETAIL_STLP_BITSET_TEST_DECL (retail calls 0x00410DE0); bitset pad _pad_0x21[0x23] removed.
// Naming wave 18: the interface-presence stand-in CPresenceBits is std::bitset<274> (STLport 4.6); its test is ?test@?$bitset@$0BBC@@std@@QBE_NI@Z (name-only).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0059D9C2, 78 bytes.
// Callees: std::bitset::test @ 0x00410DE0 (the thing's interface presence bits at +0x20), CVectorMap<ETCInterfaceType,
//          CTCBase*>::LowerBound @ 0x0040F020 (the sorted interface map at +0x44),
//          CTCHeroAttachableAppearanceModifiers::AddAppearanceModifierWithReplace(long) @ 0x00706950.
//
// Shape: the thing's inline GetInterface(type, &out) of the landed 0x007BEA08 / 0x007504E0 family -- bitset test, key in
// the `push ecx` frame slot, the map pointer a named local (`add esi,44h`), a miss (end or larger key) taking the end
// entry, the component handed out through the OUT parameter so it stays in eax (`mov eax,[eax+4] ... mov ecx,eax`).
// Frame pointer in a size TU: `#pragma optimize("y", off)` + `optimize("s", on)`; under "s" the helper is only inlined
// when `__forceinline` (otherwise VC calls it out of line). +4 is CTCBase::ParentThing. The card's +0x28 word is named
// from use. Local model (no engine headers for CThing / the card def).
#include "engine/CTCBase.h"
// The local view is named CTCInventoryTrade_Retail so it does not shadow the PDB header engine/CTCInventoryTrade.h.
#pragma optimize("y", off)
#pragma optimize("s", on)
class CTCBase;

#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#include "engine/CVectorMap.h"

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing { public:
    unsigned char _pad_0x00[0x20];
    std::bitset<274> m_present;                  // +0x20
    CInterfaceMap m_interfaces;               // +0x44
    template <class T> __forceinline bool GetInterface(long type, T** out)
    {
        if (m_present.test(type)) {
            ETCInterfaceType key = (ETCInterfaceType)(type);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::value_type* it = map->LowerBound(key);
            if (it == map->end() || type < it->first)
                it = map->end();
            *out = (T*)it->second;
            return true;
        }
        return false;
    }
};

class CTCHeroAttachableAppearanceModifiers { public:
    void AddAppearanceModifierWithReplace(long modifierDefIndex);   // 0x00706950
};

class CHairCardDef { public:
    unsigned char _pad_0x00[0x28];
    long AppearanceModifier;                  // +0x28
};

struct CTCInventoryTrade_Retail : CTCBase {
    void ApplyHairCard(const CHairCardDef* card);
};

void CTCInventoryTrade_Retail::ApplyHairCard(const CHairCardDef* card)
{
    CTCHeroAttachableAppearanceModifiers* modifiers;
    if (ParentThing->GetInterface(0x5e, &modifiers))
        modifiers->AddAppearanceModifierWithReplace(card->AppearanceModifier);
}