#ifndef FABLE_ENGINE_CINPUTPROCESSCONTROLCREATUREACTIVATEZTARGET_H
#define FABLE_ENGINE_CINPUTPROCESSCONTROLCREATUREACTIVATEZTARGET_H
// Retail opt-in (chunk 873, row 0x00488D10 CPlayer::InitInterfaces): RTTI CInputProcessControlCreatureActivateZTarget : CInputProcessGameBase (ghidra_out/rtti_bases.tsv), vtable 0x012375DC;
// retail sizeof 0x28 (`push 28h ; call operator new` before the construction in InitInterfaces, CPlayer::InputProcessControlCreatureActivateZTarget +0x188).
// The constructor is inline in retail (FableWin.pdb ??0CInputProcessControlCreatureActivateZTarget@@QAE@ABVCInputProcessGameBaseInit@@@Z, no Ego_r.pdb or retail
// out-of-line copy): the base constructor (0x00687A30), then this vtable.
// Member names: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv). Virtuals other than the inherited destructor are not modelled.
#include "rebuild_abi.h"
#include "engine/CInputProcessGameBase.h"
class CInputProcessControlCreatureActivateZTarget : public CInputProcessGameBase {
public:
#ifdef FABLE_RETAIL_INPUTPROCESS_ZTARGET
    virtual void ProcessInput(const CInputEvent& event, CProcessedInput& processed_input);
#endif
    CInputProcessControlCreatureActivateZTarget(const CInputProcessGameBaseInit& init) : CInputProcessGameBase(init) { }
#ifdef FABLE_RETAIL_INPUTPROCESS_VTABLE
    // Opt-in (NF477, r467d / r459a): the retail virtual list, declaration only. Retail vftable 0x012375DC, 9 counted function slots
    // (RTTI locator at vftable - 4, then consecutive slots at .text function starts); listed: each slot whose target differs from the
    // base class table, named by the base slot it fills (order from retail, spellings from Ego_r.pdb / FableWin.pdb).
public:
#if !(defined(FABLE_RETAIL_INPUTPROCESS_ZTARGET))
    virtual void ProcessInput(const CInputEvent& event, CProcessedInput& processed_input);           // slot 4, 0x00688260
#endif
public:
#endif
};
FABLE_STATIC_ASSERT_AT(17, sizeof(CInputProcessControlCreatureActivateZTarget) == 0x28);
#endif // FABLE_ENGINE_CINPUTPROCESSCONTROLCREATUREACTIVATEZTARGET_H
