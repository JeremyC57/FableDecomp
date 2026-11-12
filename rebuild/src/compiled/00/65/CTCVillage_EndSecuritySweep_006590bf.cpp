// CTCVillage::EndSecuritySweep() -- tail call to the private InvalidateAndClearAllSecuritySweepTasks (0x006590A0).
// Leaf: ?EndSecuritySweep@CTCVillage@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x006590BF, 5 bytes: `jmp 0x006590A0`.
// Name (agentNF32, 2026-11-12; was the forwarder stand-in _Forward / ForwardTarget_006590a0, catalog _global::TailJmp_InvalidateAnd
//   ClearAllSecuritySweepTasks_006590bf): Ego_r.pdb 0x0069D8D1 ?EndSecuritySweep@CTCVillage@@QAEXXZ is the same 5-byte `jmp` to its
//   0x0069D86E ?InvalidateAndClearAllSecuritySweepTasks@CTCVillage@@AAEXXZ (strict signature f448c9f6d17c6e3f in both exes,
//   ghidra_out/naming_stragglers); FableWin.pdb 0x02161BAA (17-byte debug one-call wrapper). The landed 0x007E2A60
//   CTCFineDialogue::GlobalUpdate binds this address as ?EndSecuritySweep@CTCVillage@@QAEXXZ through engine/CTCVillage.h.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCVILLAGE
#define FABLE_RETAIL_CTCVILLAGE_FINEDIALOGUE
#include "engine/ETCInterfaceType.h"
#include "engine/CTCVillage.h"

void CTCVillage::EndSecuritySweep()
{
    InvalidateAndClearAllSecuritySweepTasks();
}
