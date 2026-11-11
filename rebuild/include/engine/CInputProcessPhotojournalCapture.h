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
#ifdef FABLE_RETAIL_CINPUTPROCESSPHOTOJOURNALCAPTURE_VSLOTS
    // agentFAM03 (2026-11-11): retail virtuals missing from this declaration, declaration only (no layout change).
    // Slots: the TLC vtable aligned with the Xbox 360 Test PDB order on TLC-named slots; spellings: Ego_r / FableWin name sets.
public:
    virtual bool HasFrameUpdate() const;  // slot 9, 0x00486C20
public:
    virtual void FrameUpdate();  // slot 10, 0x00691AD0
public:
#endif
};
FABLE_STATIC_ASSERT(sizeof(CInputProcessPhotojournalCapture) == 0x28);
#endif // FABLE_ENGINE_CINPUTPROCESSPHOTOJOURNALCAPTURE_H
