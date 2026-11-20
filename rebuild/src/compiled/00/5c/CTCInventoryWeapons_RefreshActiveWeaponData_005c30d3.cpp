// CTCInventoryWeapons::RefreshActiveWeaponData() -- re-applies the base inventory's per-item data update to each active weapon (melee, then ranged) that is still alive
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005C30D3, 71 bytes.
// Callees: IS_THING_ALIVE<CIntelligentPointer> @ 0x004CC340 (`__fastcall`, pointer in ecx), CBaseIntelligentPointer::GetPItem
//          @ 0x00A01B50, the base inventory's per-item update @ 0x005BF414 (takes the CThing*, checks its interface 0x1A;
//          manifest label "CTCInventoryStats::BuildRenownString" is propagated -- name here descriptive).
//
// Shape: two identical blocks, each forming the pointer's address once (`lea edi,[esi+134h]` / `lea edi,[esi+141h]`) for
// both the liveness test and GetPItem; the item is pushed before `mov ecx,esi`. Size TU (`#pragma optimize("s", on)`).
// PActiveMeleeWeapon / PActiveRangedWeapon are PDB +0x14C / +0x159, retail +0x134 / +0x141 (unaligned in both).
// Local model (no engine header for CTCInventoryWeapons).
// Naming (NF191, 2026-11-20, q4492): the row-local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared
//   engine/CIntelligentPointer.h (members CIntelligentPointer<CThing>, the Ego_r.pdb member type) and the IS_THING_ALIVE template
//   declaration (the out-of-line instance 0x004CC340, as the landed CTCInventoryWeapons rows declare it): the liveness tests bind the
//   FableWin.pdb ??$IS_THING_ALIVE@V?$CIntelligentPointer@VCThing@@@@@@YI_NABV?$CIntelligentPointer@VCThing@@@@@Z. Other row-local
//   models unchanged (in place).
#pragma optimize("s", on)
class CThing;
class CBaseObject;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
template <class T> bool FABLE_FASTCALL IS_THING_ALIVE(const T& thing);   // 0x004CC340
struct CTCInventoryBase_Retail {
    virtual void v00();
    unsigned char _pad_0x04[0x130 - 4];
    void RefreshItemData(CThing* item);       // 0x005BF414
};
namespace NHeroInformationScreens {
class CBase { public: virtual void v00(); };
}
class CTCInventoryWeapons : public CTCInventoryBase_Retail, public NHeroInformationScreens::CBase { public:
    CIntelligentPointer<CThing> PActiveMeleeWeapon;   // +0x134
    long ActiveMeleeWeaponDefIndex;           // +0x13C
    bool MeleeWeaponCarriedOnLoad;            // +0x140
    CIntelligentPointer<CThing> PActiveRangedWeapon;  // +0x141
    void RefreshActiveWeaponData();
};
void CTCInventoryWeapons::RefreshActiveWeaponData()
{
    if (IS_THING_ALIVE(PActiveMeleeWeapon))
        RefreshItemData(((CThing*)((CBaseIntelligentPointer*)&PActiveMeleeWeapon)->GetPItem()));
    if (IS_THING_ALIVE(PActiveRangedWeapon))
        RefreshItemData(((CThing*)((CBaseIntelligentPointer*)&PActiveRangedWeapon)->GetPItem()));
}