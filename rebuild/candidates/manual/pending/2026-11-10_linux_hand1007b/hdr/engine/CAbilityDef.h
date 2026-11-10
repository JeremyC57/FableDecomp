#ifndef FABLE_ENGINE_CABILITYDEF_H
#define FABLE_ENGINE_CABILITYDEF_H
// Retail model (chunk 1007b, row 0x00584B86 CTCInventory::AssignThingPC). RTTI .?AVCAbilityDef@@ : CSubDefClassBase; Ego_r.pdb
// struct_layouts_egor.tsv: CAbilityDef 44 bytes, `EHeroAbility Ability` at +40 (retail reads it at +0x28: `mov ecx,[ecx+28h]`,
// 0x00584E4C). StatGetTypeName is FableWin.pdb ?StatGetTypeName@CAbilityDef@@SI?AVCCharString@@XZ, inline in the release build:
// the "CAbilityDef" literal the landed GetSubDef<CAbilityDef> 0x005936C2 passes to GetPSubDefInfo. The virtual Transfer / Copy /
// GetSizeofClass overrides of the landed 0x005936C2 local model are not declared (no row calls them). Needs
// FABLE_RETAIL_DEF_CLASS_BASE.
#include "fable_string.h"
#include "engine/CSubDefClassBase.h"
#include "engine/EHeroAbility.h"
class CAbilityDef : public CSubDefClassBase { public:
    static CCharString __fastcall StatGetTypeName() { return CCharString("CAbilityDef", -1); }
    EHeroAbility Ability;                                                // +0x28
};
FABLE_STATIC_ASSERT(offsetof(CAbilityDef, Ability) == 0x28);
FABLE_STATIC_ASSERT(sizeof(CAbilityDef) == 0x2c);
#endif
