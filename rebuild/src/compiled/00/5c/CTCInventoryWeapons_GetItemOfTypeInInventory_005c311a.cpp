// CTCInventoryWeapons::GetItemOfTypeInInventory(long itemType) -- the active melee or ranged weapon thing when it is alive and of that type, else the base inventory's freshly created item
// Retry 10b (getsw sweep, FIXES B-15): the def index is read through an inline getter `GetDefIndex()`; the plain member read gives the parked register/scheduling difference.
// Leaf: GetItemOfTypeInInventory@CTCInventoryWeapons
// Leaf: GetItemOfTypeInInventory@CTCInventoryWeapons
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005C311A, length 102 bytes (`ret 4`). Size TU (`#pragma optimize("s", on)`, as the
// landed neighbour RefreshActiveWeaponData 0x005C30D3).
// Callees: IS_THING_ALIVE<CIntelligentPointer> @ 0x004CC340 (`__fastcall`, pointer in ecx), CBaseIntelligentPointer::GetPItem
// @ 0x00A01B50, CTCInventoryBase::GetItemOfTypeInInventory(long) @ 0x005BF3F1 (qualified base call).
// Shape: each weapon's address is formed once in esi (`lea esi,[edi+134h]` / `lea esi,[edi+141h]`); the two hit arms tail-merge
// into one `GetPItem` call on esi; the def index is CThing's unsigned short +0x8C.
// Layout: PActiveMeleeWeapon / PActiveRangedWeapon at retail +0x134 / +0x141 (PDB +0x14C / +0x159; unaligned in both).
// Naming (NF191, 2026-11-20, q4492): the row-local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared
//   engine/CIntelligentPointer.h (members CIntelligentPointer<CThing>, the Ego_r.pdb member type) and the IS_THING_ALIVE template
//   declaration (the out-of-line instance 0x004CC340, as the landed CTCInventoryWeapons rows declare it): the liveness tests bind the
//   FableWin.pdb ??$IS_THING_ALIVE@V?$CIntelligentPointer@VCThing@@@@@@YI_NABV?$CIntelligentPointer@VCThing@@@@@Z. Other row-local
//   models unchanged (in place).
#pragma optimize("s", on)
class CThing { public:
    unsigned char  _pad00[0x8c];
    unsigned short DefIndex;                  // +0x8c
    unsigned short GetDefIndex() const { return DefIndex; }
};
class CBaseObject;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
template <class T> bool FABLE_FASTCALL IS_THING_ALIVE(const T& thing);   // 0x004CC340
struct CTCInventoryBase_Retail {
    virtual void v00();
    unsigned char _pad_0x04[0x130 - 4];
    CThing* GetItemOfTypeInInventory(long itemType);   // 0x005BF3F1
};
namespace NHeroInformationScreens {
class CBase { public: virtual void v00(); };
}
class CTCInventoryWeapons : public CTCInventoryBase_Retail, public NHeroInformationScreens::CBase { public:
    CIntelligentPointer<CThing> PActiveMeleeWeapon;   // +0x134
    long ActiveMeleeWeaponDefIndex;           // +0x13C
    bool MeleeWeaponCarriedOnLoad;            // +0x140
    CIntelligentPointer<CThing> PActiveRangedWeapon;  // +0x141
    CThing* GetItemOfTypeInInventory(long itemType);
};
CThing* CTCInventoryWeapons::GetItemOfTypeInInventory(long itemType)
{
    if (IS_THING_ALIVE(PActiveMeleeWeapon) && ((CThing*)((CBaseIntelligentPointer*)&PActiveMeleeWeapon)->GetPItem())->GetDefIndex() == itemType)
        return ((CThing*)((CBaseIntelligentPointer*)&PActiveMeleeWeapon)->GetPItem());
    if (IS_THING_ALIVE(PActiveRangedWeapon) && ((CThing*)((CBaseIntelligentPointer*)&PActiveRangedWeapon)->GetPItem())->GetDefIndex() == itemType)
        return ((CThing*)((CBaseIntelligentPointer*)&PActiveRangedWeapon)->GetPItem());
    return CTCInventoryBase_Retail::GetItemOfTypeInInventory(itemType);
}