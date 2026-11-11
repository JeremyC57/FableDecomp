#ifndef FABLE_ENGINE_CTCCLOCK_H
#define FABLE_ENGINE_CTCCLOCK_H
// NEW (agentNF11, row 0x00732850 CTCClock::OnInitialActivate): the clock component. RTTI .?AVCTCClock@@ : CTCBase
// (ghidra_out/rtti_bases.tsv). Members: Ego_r.pdb CTCClock (struct_layouts_egor.tsv, 0x14, no STL member: the same offsets in
// retail; OnInitialActivate stores LastTimeUpdated `mov dword ptr [ebx+10h],3F000000h` at 0x00732859). Methods: the Ego_r.pdb /
// FableWin.pdb spelling the row defines (?OnInitialActivate@CTCClock@@UAEXXZ: public virtual, CTCBase slot 6). Needs
// FABLE_RETAIL_CTCBASE and FABLE_RETAIL_DEF_CLASS_BASE.
#include <stddef.h>
#include "rebuild_abi.h"
#include "engine/CTCBase.h"
#include "engine/CDefPointer.h"
class CClockDef;
class CTCClock : public CTCBase {
public:
    virtual void OnInitialActivate();                                    // slot 6 0x00732850
private:
    CDefPointer<const CClockDef> PDef;                                   // +0x0c
    float                        LastTimeUpdated;                        // +0x10
#ifdef FABLE_RETAIL_CTCCLOCK_VSLOTS
    // agentFAM03 (2026-11-11): retail virtuals missing from this declaration, declaration only (no layout change).
    // Slots: the TLC vtable aligned with the Xbox 360 Test PDB order on TLC-named slots; spellings: Ego_r / FableWin name sets.
public:
    virtual bool HasFrameUpdate() const;  // slot 8, 0x00732BC0
public:
#endif
};
FABLE_STATIC_ASSERT(sizeof(CTCClock) == 0x14);
#endif
