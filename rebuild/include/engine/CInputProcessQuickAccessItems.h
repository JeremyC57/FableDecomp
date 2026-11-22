#ifndef FABLE_ENGINE_CINPUTPROCESSQUICKACCESSITEMS_H
#define FABLE_ENGINE_CINPUTPROCESSQUICKACCESSITEMS_H
// Retail opt-in (chunk 873, row 0x00488D10 CPlayer::InitInterfaces): RTTI CInputProcessQuickAccessItems : CInputProcessGameBase (ghidra_out/rtti_bases.tsv), vtable 0x01237AE0;
// retail sizeof 0x28 (`push 28h ; call operator new` before the construction in InitInterfaces, CPlayer::InputProcessQuickAccessItems +0x160).
// The constructor is inline in retail (FableWin.pdb ??0CInputProcessQuickAccessItems@@QAE@ABVCInputProcessGameBaseInit@@@Z, no Ego_r.pdb or retail
// out-of-line copy): the base constructor (0x00687A30), then this vtable.
// Member names: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv). Virtuals other than the inherited destructor are not modelled.
#include "rebuild_abi.h"
#include "engine/CInputProcessGameBase.h"
#ifdef FABLE_RETAIL_CINPUTPROCESSQUICKACCESSITEMS_UPDATE
class CProcessedInput;
#endif
class CInputProcessQuickAccessItems : public CInputProcessGameBase {
public:
    CInputProcessQuickAccessItems(const CInputProcessGameBaseInit& init) : CInputProcessGameBase(init) { }
#ifdef FABLE_RETAIL_CINPUTPROCESSQUICKACCESSITEMS_PROCESS_PASS26
    virtual void ProcessInput(const CInputEvent& event,CProcessedInput& processed_input);
private:
    // Both original PUBLIC/GDATA records: private static longs.
    static long ItemIndex;
    static long ButtonIndex;
public:
#endif
#ifdef FABLE_RETAIL_CINPUTPROCESSQUICKACCESSITEMS_UPDATE
    // NF55 opt-in (2026-11-17, row 0x0068E450): Ego_r.pdb / FableWin.pdb ?Update@CInputProcessQuickAccessItems@@UAEXAAVCProcessedInput@@@Z
    // (0x0068E450) and the private ?CancelQuickAccessCharge@CInputProcessQuickAccessItems@@AAEXXZ (0x0068E3C0). Declarations only: the
    // vtable already exists in retail; no layout change.
    virtual void Update(CProcessedInput& processed);                     // 0x0068E450
private:
    void CancelQuickAccessCharge();                                      // 0x0068E3C0
public:
#endif
};
FABLE_STATIC_ASSERT_AT(14, sizeof(CInputProcessQuickAccessItems) == 0x28);
#endif // FABLE_ENGINE_CINPUTPROCESSQUICKACCESSITEMS_H
