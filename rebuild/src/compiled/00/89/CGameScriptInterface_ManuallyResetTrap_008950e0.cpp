// CGameScriptInterface::ManuallyResetTrap(const CScriptThing& trap) const -- the thing's trap component, if any, is reset by hand
// Leaf: ?ManuallyResetTrap@CGameScriptInterface@@UBE_NABVCScriptThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x008950E0. Identity (NF435, 2026-11-25): the retail RTTI vftable placement of
//   CGameScriptInterface (vtable 0x01260F0C, ghidra_out/labels_rtti_port.tsv) names this PDB member here, and Ego_r.pdb /
//   FableWin.pdb list the symbol (two sources); defined on the real class of engine/CGameScriptInterface.h (FABLE_RETAIL_NSCRIPT).
// FableWin's /Od body: trap.GetPThing() (CScriptThing slot 11, +0x2c), the null test, then ??$GetTC@VCTCTrapBase@@@CThing@@QAE_NPAPAVCTCTrapBase@@@Z
//   (inline in retail: the UsingComponents bit 0x7D TCI_TRAP, CVectorMap::LowerBound 0x0040F020, the end-or-greater test) and
//   ?ManualReset@CTCTrapBase@@QAEXXZ (retail 0x00787C0C, the call target); true, else false. The DoErrorMessage branch is debug-only.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTCTRAPBASE
#include "engine/CGameScriptInterface.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CThing.h"
#include "engine/CTCTrapBase.h"

bool CGameScriptInterface::ManuallyResetTrap(const CScriptThing& trap) const
{
    CThing* thing = trap.GetPThing();
    if (thing) {
        CTCTrapBase* trap_tc;
        if (thing->GetTC(&trap_tc)) {
            trap_tc->ManualReset();
            return true;
        }
    }
    return false;
}
