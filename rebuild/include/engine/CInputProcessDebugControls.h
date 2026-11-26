#ifndef FABLE_ENGINE_CINPUTPROCESSDEBUGCONTROLS_H
#define FABLE_ENGINE_CINPUTPROCESSDEBUGCONTROLS_H
// Retail opt-in (chunk 873, row 0x00488D10 CPlayer::InitInterfaces): RTTI CInputProcessDebugControls : CInputProcessGameBase (ghidra_out/rtti_bases.tsv), vtable 0x01237F9C;
// retail sizeof 0x30 (`push 30h ; call operator new` before the construction in InitInterfaces, CPlayer::InputProcessDebugControls +0x15C).
// The constructor is inline in retail (FableWin.pdb ??0CInputProcessDebugControls@@QAE@ABVCInputProcessGameBaseInit@@@Z, no Ego_r.pdb or retail
// out-of-line copy): the base constructor (0x00687A30), then this vtable and the members below.
// PCreature (a CIntelligentPointer<const CThing>) default-constructed (0x00A01B10, then vtable 0x01237F1C).
// Member names: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv). Virtuals other than the inherited destructor are not modelled.
#include "rebuild_abi.h"
#include "engine/CInputProcessGameBase.h"
#include "engine/CIntelligentPointer.h"   // FABLE_RETAIL_CINTELLIGENTPOINTER
class CThing;
class CInputProcessDebugControls : public CInputProcessGameBase {
public:
    CInputProcessDebugControls(const CInputProcessGameBaseInit& init) : CInputProcessGameBase(init) { }
    CIntelligentPointer<const CThing> PCreature;                    // +0x28
#ifdef FABLE_RETAIL_INPUTPROCESS_VTABLE
    // Opt-in (NF477, r467d / r459a): the retail virtual list, declaration only. Retail vftable 0x01237F9C, 9 counted function slots
    // (RTTI locator at vftable - 4, then consecutive slots at .text function starts); listed: each slot whose target differs from the
    // base class table, named by the base slot it fills (order from retail, spellings from Ego_r.pdb / FableWin.pdb).
public:
    virtual void Init();                                                                             // slot 2, 0x00691FE0
    virtual void ProcessInput(const CInputEvent& event, CProcessedInput& processed_input);           // slot 4, 0x00691FD0
public:
#endif
};
FABLE_STATIC_ASSERT_AT(18, sizeof(CInputProcessDebugControls) == 0x30);
#endif // FABLE_ENGINE_CINPUTPROCESSDEBUGCONTROLS_H
