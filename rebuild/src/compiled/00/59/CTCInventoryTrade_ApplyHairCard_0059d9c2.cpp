// CTCInventoryTrade::ApplyHairCard(const CHairCardDef*) -- if the owning thing has the attachable-appearance-modifiers component (interface 0x5E), applies the card's appearance modifier with replacement
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

struct CPresenceBits {
    bool test(long bit);                      // 0x00410DE0 (std::bitset::test)
};

struct CInterfaceMap {                        // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);     // 0x0040F020
};

class CThing { public:
    unsigned char _pad_0x00[0x20];
    CPresenceBits m_present;                  // +0x20
    unsigned char _pad_0x21[0x23];
    CInterfaceMap m_interfaces;               // +0x44
    template <class T> __forceinline bool GetInterface(long type, T** out)
    {
        if (m_present.test(type)) {
            long key = type;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&key);
            if (it == map->last || type < it->type)
                it = map->last;
            *out = (T*)it->component;
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