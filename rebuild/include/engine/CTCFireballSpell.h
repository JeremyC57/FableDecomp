#ifndef FABLE_ENGINE_CTCFIREBALLSPELL_H
#define FABLE_ENGINE_CTCFIREBALLSPELL_H
// NEW (chunk 890, row 0x0056AD40 CTCBoastUI::Activate). Without the opt-in the header only names the class.
#ifdef FABLE_RETAIL_CTCFIREBALLSPELL
// Retail opt-in: RTTI .?AVCTCFireballSpell@@ : CTCBase (ghidra_out/rtti_bases.tsv; needs FABLE_RETAIL_CTCBASE, forced below). Members:
// FableWin.pdb ?StatGetInterfaceType@CTCFireballSpell@@SI?AW4ETCInterfaceType@@XZ (0x02154ABC; inline in retail: Activate looks up
// TC 157 = TCI_FIREBALL_SPELL, `mov dword ptr [esp+24h],9Dh` at 0x0056BDB5) and Ego_r.pdb 0x007C7D90 / FableWin.pdb 0x02721140
// ?CancelMove@CTCFireballSpell@@QAEXXZ (out of line in retail at 0x007C8FD0, Activate 0x0056BDD6). The data members are not modelled
// (no row reads them yet), so the class is used through pointers only.
#ifndef FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE
#endif
#include "rebuild_abi.h"
#include "engine/CTCBase.h"
#include "engine/ETCInterfaceType.h"
class CTCFireballSpell : public CTCBase {
public:
    static ETCInterfaceType FABLE_FASTCALL StatGetInterfaceType() { return TCI_FIREBALL_SPELL; }
    void CancelMove();                                                   // 0x007C8FD0
#ifdef FABLE_RETAIL_CTCFIREBALLSPELL_VSLOTS
    // agentFAM03 (2026-11-11): retail virtuals the declarations above lack, declaration only (no layout change; the
    // vtable already exists). Slots from the TLC vtable aligned with the Xbox 360 Test PDB order on TLC-named slots;
    // spellings from the Ego_r / FableWin name sets.
public:
    virtual bool HasFrameUpdate() const;  // slot 8, 0x007C95B0
public:
#endif
};
#else
class CTCFireballSpell;
#endif
#endif // FABLE_ENGINE_CTCFIREBALLSPELL_H
