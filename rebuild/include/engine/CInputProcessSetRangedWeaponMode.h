#ifndef FABLE_ENGINE_CINPUTPROCESSSETRANGEDWEAPONMODE_H
#define FABLE_ENGINE_CINPUTPROCESSSETRANGEDWEAPONMODE_H
// Retail opt-in (chunk 873, row 0x00488D10 CPlayer::InitInterfaces): RTTI CInputProcessSetRangedWeaponMode : CInputProcessGameBase (ghidra_out/rtti_bases.tsv), vtable 0x01237B30;
// retail sizeof 0x28 (`push 28h ; call operator new` before the construction in InitInterfaces, CPlayer::InputProcessSetRangedWeaponMode +0x1A0).
// The constructor is inline in retail (FableWin.pdb ??0CInputProcessSetRangedWeaponMode@@QAE@ABVCInputProcessGameBaseInit@@@Z, no Ego_r.pdb or retail
// out-of-line copy): the base constructor (0x00687A30), then this vtable.
// Member names: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv). Virtuals other than the inherited destructor are not modelled.
#include "rebuild_abi.h"
#include "engine/CInputProcessGameBase.h"
class CInputProcessSetRangedWeaponMode : public CInputProcessGameBase {
public:
    CInputProcessSetRangedWeaponMode(const CInputProcessGameBaseInit& init) : CInputProcessGameBase(init) { }
};
FABLE_STATIC_ASSERT(sizeof(CInputProcessSetRangedWeaponMode) == 0x28);
#endif // FABLE_ENGINE_CINPUTPROCESSSETRANGEDWEAPONMODE_H
