#ifndef FABLE_ENGINE_CTCMULTIARROW_H
#define FABLE_ENGINE_CTCMULTIARROW_H
// CTCMultiArrow (new shared header, chunk 1004, row 0x005CA68E CTCInventoryWeapons::SetupActiveWeaponsOnCarrier): the multi-arrow
// spell component over FABLE_RETAIL_CTCBASE. Ego_r.pdb members (struct_layouts_egor.tsv, sizeof 0x4C): PDef +0x0C
// (CDefPointer<const CSpecialAbilitiesMultiArrowDef>), NumberOfShots +0x10, NumberInVolley +0x14 .. CutSceneCancelled +0x48; only the
// first two are laid out (PDef opaque), the rest are not modelled. FableWin.pdb ?GetShotsRemaining@CTCMultiArrow@@QBEHXZ is inline in
// retail (`cmp dword ptr [edi+10h],ebx ; jle`, 0x005CAC43). StatGetInterfaceType: TCI_MULTI_ARROW (136; `mov edi,88h`, 0x005CA730).
// Out-of-line methods (Ego_r.pdb + FableWin.pdb spellings): ?ApplyEffectOnWeapon@CTCMultiArrow@@QAEXPAVCThing@@@Z (retail 0x007CC060)
// and ?RemoveEffectOnWeapon@CTCMultiArrow@@QAEXPAVCThing@@@Z (0x007CC120). Declarations only.
#ifndef FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE
#endif
#include <stddef.h>
#include "rebuild_abi.h"
#include "engine/CTCBase.h"
#include "engine/ETCInterfaceType.h"
class CThing;
class CTCMultiArrow : public CTCBase {
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_MULTI_ARROW; }
    void ApplyEffectOnWeapon(CThing* weapon);                // 0x007CC060
    void RemoveEffectOnWeapon(CThing* weapon);               // 0x007CC120
    int GetShotsRemaining() const { return NumberOfShots; }
    // Public as the other partial retail models (CTCHero): the layout is asserted below.
    unsigned char PDef[4];                                   // +0x0C CDefPointer<const CSpecialAbilitiesMultiArrowDef>
    long NumberOfShots;                                      // +0x10
#ifdef FABLE_RETAIL_CTCMULTIARROW_VSLOTS
    // agentFAM03 (2026-11-11): retail virtuals missing from this declaration, declaration only (no layout change).
    // Slots: the TLC vtable aligned with the Xbox 360 Test PDB order on TLC-named slots; spellings: Ego_r / FableWin name sets.
public:
    virtual bool HasFrameUpdate() const;  // slot 8, 0x007CC980
public:
#endif
#ifdef FABLE_RETAIL_CTCMULTIARROW_VSLOTS2
// agentFAM04 (2026-11-11): retail virtuals and the non-virtual they call (Ego_r.pdb / FableWin.pdb spellings; TLC vtable slots aligned with the Xbox 360 Test PDB order). Declarations only (no layout change).
public:
    virtual void OnRemove();                                             // slot 2, 0x007CC480 (tail call to FinalCleanup)
    virtual void OnKill();                                               // slot 14, 0x007CC470 (tail call to FinalCleanup)
protected:
    void FinalCleanup();                                                 // 0x007CC410 ?FinalCleanup@CTCMultiArrow@@IAEXXZ
public:
#endif // FABLE_RETAIL_CTCMULTIARROW_VSLOTS2
};
FABLE_STATIC_ASSERT(offsetof(CTCMultiArrow, NumberOfShots) == 0x10);
#endif // FABLE_ENGINE_CTCMULTIARROW_H
