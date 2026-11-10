#ifndef FABLE_ENGINE_CTCSUMMONSPELL_H
#define FABLE_ENGINE_CTCSUMMONSPELL_H
// NEW (chunk 1008b, row 0x005EE49A CTCInventoryAbilities::IsAbleToUseAbility). Without the opt-in the header only names the class.
#ifdef FABLE_RETAIL_CTCSUMMONSPELL
// Retail opt-in: RTTI .?AVCTCSummonSpell@@ : CTCBase (needs FABLE_RETAIL_CTCBASE, forced below). Members:
// FableWin.pdb ?StatGetInterfaceType@CTCSummonSpell@@SI?AW4ETCInterfaceType@@XZ (inline in retail: IsAbleToUseAbility looks up
// TC 145 = TCI_SUMMON_SPELL, `mov ebx,91h` at 0x005EE66A) and Ego_r.pdb / FableWin.pdb ?GetResetCreatureManaCost@CTCSummonSpell@@QBEJXZ
// (the class's only QBEJXZ member in both name sets), out of line in retail at 0x0079A610 between FinalCleanup 0x0079A600 and the
// ctor 0x0079A620: `mov eax,[ecx+0Ch] ; mov eax,[eax+54h] ; ret` = PDef (Ego_r +0x0C CDefPointer<const CSpecialAbilitiesSummonSpellDef>)
// -> ResetCreatureManaCost (Ego_r +0x5C; retail +0x54: CParentDefClassBase 0x3C, TimeOfSummon a 12-byte STLport vector). The data
// members are not modelled (no row reads them), so the class is used through pointers only.
#ifndef FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE
#endif
#include "rebuild_abi.h"
#include "engine/CTCBase.h"
#include "engine/ETCInterfaceType.h"
class CTCSummonSpell : public CTCBase {
public:
    static ETCInterfaceType FABLE_FASTCALL StatGetInterfaceType() { return TCI_SUMMON_SPELL; }
    long GetResetCreatureManaCost() const;                               // 0x0079A610
};
#else
class CTCSummonSpell;
#endif
#endif // FABLE_ENGINE_CTCSUMMONSPELL_H
