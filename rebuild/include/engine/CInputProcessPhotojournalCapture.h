#ifndef FABLE_ENGINE_CINPUTPROCESSPHOTOJOURNALCAPTURE_H
#define FABLE_ENGINE_CINPUTPROCESSPHOTOJOURNALCAPTURE_H
// Retail opt-in (chunk 873, row 0x00488D10 CPlayer::InitInterfaces): RTTI CInputProcessPhotojournalCapture : CInputProcessGameBase (ghidra_out/rtti_bases.tsv), vtable 0x01237EC8;
// retail sizeof 0x28 (`push 28h ; call operator new` before the construction in InitInterfaces, CPlayer::InputProcessPhotojournalCapture +0x200).
// The constructor is inline in retail (FableWin.pdb ??0CInputProcessPhotojournalCapture@@QAE@ABVCInputProcessGameBaseInit@@@Z, no Ego_r.pdb or retail
// out-of-line copy): the base constructor (0x00687A30), then this vtable.
// Member names: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv). Virtuals other than the inherited destructor are not modelled.
#include "rebuild_abi.h"
#include "engine/CInputProcessGameBase.h"
class CInputProcessPhotojournalCapture : public CInputProcessGameBase {
public:
    CInputProcessPhotojournalCapture(const CInputProcessGameBaseInit& init) : CInputProcessGameBase(init) { }
#ifdef FABLE_RETAIL_CINPUTPROCESSPHOTOJOURNALCAPTURE_PROCESSINPUT
    // Opt-in (NF62, row 0x00691AE0): Ego_r.pdb ?ProcessInput@CInputProcessPhotojournalCapture@@UAEXABVCInputEvent@@AAVCProcessedInput@@@Z.
    // Declaration only.
    virtual void ProcessInput(const CInputEvent& event, CProcessedInput& processed_input);   // 0x00691AE0
#endif
#ifdef FABLE_RETAIL_CINPUTPROCESSPHOTOJOURNALCAPTURE_VSLOTS
    // agentFAM03 (2026-11-11): retail virtuals missing from this declaration, declaration only (no layout change).
    // Slots: the TLC vtable aligned with the Xbox 360 Test PDB order on TLC-named slots; spellings: Ego_r / FableWin name sets.
public:
    virtual bool HasFrameUpdate() const;  // slot 9, 0x00486C20
public:
    virtual void FrameUpdate();  // slot 10, 0x00691AD0
public:
#endif
#ifdef FABLE_RETAIL_INPUTPROCESS_VTABLE
    // Opt-in (NF477, r467d / r459a): the retail virtual list, declaration only. Retail vftable 0x01237EC8, 11 counted function slots
    // (RTTI locator at vftable - 4, then consecutive slots at .text function starts); listed: each slot whose target differs from the
    // base class table, named by the base slot it fills (order from retail, spellings from Ego_r.pdb / FableWin.pdb).
public:
    virtual void Init();                                                                             // slot 2, 0x00691AC0
#if !(defined(FABLE_RETAIL_CINPUTPROCESSPHOTOJOURNALCAPTURE_PROCESSINPUT))
    virtual void ProcessInput(const CInputEvent& event, CProcessedInput& processed_input);           // slot 4, 0x00691AE0
#endif
    virtual bool ProcessWhilePaused() const;                                                         // slot 7, 0x00486C30
#if !(defined(FABLE_RETAIL_CINPUTPROCESSPHOTOJOURNALCAPTURE_VSLOTS))
    virtual bool HasFrameUpdate() const;                                                             // slot 9, 0x00486C20
#endif
#if !(defined(FABLE_RETAIL_CINPUTPROCESSPHOTOJOURNALCAPTURE_VSLOTS))
    virtual void FrameUpdate();                                                                      // slot 10, 0x00691AD0
#endif
public:
#endif
};
FABLE_STATIC_ASSERT_AT(23, sizeof(CInputProcessPhotojournalCapture) == 0x28);
#endif // FABLE_ENGINE_CINPUTPROCESSPHOTOJOURNALCAPTURE_H
