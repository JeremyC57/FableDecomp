// Leaf: ?TransferBinaryOut@CAnimComponentCombatMisc@@UBEXAAVCDataOutputStream@@@Z
// Switch route (NF318, 2026-11-23): defined on the real class of engine/CAnimComponentCombatMisc.h (class-model switch FABLE_RETAIL_ANIMCOMPONENT; was ?TransferBinaryOut@CAnimComponentCombatMisc_TransferBinaryOut@@QBEXAAVCDataOutputStream@@@Z);
//   the retail RTTI vftable placement (ghidra_out/labels_rtti_port.tsv) names this PDB member at this address. Bytes unchanged.
// CAnimComponentCombatMisc::TransferBinaryOut(CDataOutputStream&) const -- write the two melee flags
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005DB370, length 37 bytes (+ int3 padding). Vtable slot 7 of
// CAnimComponentCombatMisc (0x01255738).
// Callees: CDataOutputStream::WriteEBOOL(bool) @ 0x00993E30 (twice).
// Layout: engine/CAnimComponentCombatMisc.h (PDB names): MeleeFlourish at +0x08, MeleeKnockdown at +0x09;
// the method is defined on the real class (engine/CAnimComponentCombatMisc.h under FABLE_RETAIL_ANIMCOMPONENT).
// Shape: each bool member is widened through a cleared register (`xor eax,eax ; mov al,[..]`) and pushed;
// the stream reference is kept in edi across the two calls.
// Preamble: the define / include block of the landed precedent CAnimComponentFlags_TransferBinaryOut_005db1e0.cpp.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_ANIMCOMPONENT
#include "fable_string.h"
#include "engine/CDataOutputStream.h"
#include "engine/CAnimComponentFlags.h"
#include "engine/CAnimComponentCombatMisc.h"

void CAnimComponentCombatMisc::TransferBinaryOut(CDataOutputStream& stream) const
{
    stream.WriteEBOOL(MeleeFlourish);
    stream.WriteEBOOL(MeleeKnockdown);
}