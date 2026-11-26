#ifndef FABLE_ENGINE_CINPUTPROCESSBLOCK_H
#define FABLE_ENGINE_CINPUTPROCESSBLOCK_H
// Retail opt-in (chunk 873, row 0x00488D10 CPlayer::InitInterfaces): RTTI CInputProcessBlock : CInputProcessGameBase (ghidra_out/rtti_bases.tsv), vtable 0x01237AB8;
// retail sizeof 0x28 (`push 28h ; call operator new` before the construction in InitInterfaces, CPlayer::InputProcessBlock +0x158).
// The constructor is inline in retail (FableWin.pdb ??0CInputProcessBlock@@QAE@ABVCInputProcessGameBaseInit@@@Z, no Ego_r.pdb or retail
// out-of-line copy): the base constructor (0x00687A30), then this vtable.
// Member names: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv). Virtuals other than the inherited destructor are not modelled.
#include "rebuild_abi.h"
#include "engine/CInputProcessGameBase.h"
class CInputProcessBlock : public CInputProcessGameBase {
public:
    CInputProcessBlock(const CInputProcessGameBaseInit& init) : CInputProcessGameBase(init) { }
#ifdef FABLE_RETAIL_INPUTPROCESS_VTABLE
    // Opt-in (NF477, r467d / r459a): the retail virtual list, declaration only. Retail vftable 0x01237AB8, 9 counted function slots
    // (RTTI locator at vftable - 4, then consecutive slots at .text function starts); listed: each slot whose target differs from the
    // base class table, named by the base slot it fills (order from retail, spellings from Ego_r.pdb / FableWin.pdb).
public:
    virtual void ProcessInput(const CInputEvent& event, CProcessedInput& processed_input);           // slot 4, 0x0068E2C0
    virtual void Update(CProcessedInput& processed_input);                                           // slot 6, 0x0068E330
public:
#endif
};
FABLE_STATIC_ASSERT_AT(14, sizeof(CInputProcessBlock) == 0x28);
#endif // FABLE_ENGINE_CINPUTPROCESSBLOCK_H
