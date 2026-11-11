// CTCGatherPointBase::OnInitialActivate() -- when the gather point's thing first activates, run the leaf class's Init
// Leaf: ?OnInitialActivate@CTCGatherPointBase@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E0ADC, 5 bytes (`mov eax,[ecx]; jmp [eax+0x74]`), thiscall, vtable 0x012421E4 slot 6
//   (and the same slot of CTCGatherPointBuilding 0x01242264 / CTCGatherPointRadialArea 0x012422E4).
// Identity: CTCBase slot 6 = OnInitialActivate (engine/CTCBase.h); Ego_r.pdb ?OnInitialActivate@CTCGatherPointBase@@UAEXXZ
//   (0x004FE449, the same 5-byte forwarder) and FableWin.pdb (0x01BAC6A2). Slot 29 (+0x74) = the protected virtual Init
//   (FableWin.pdb ?Init@CTCGatherPointBase@@MAEXXZ; base body 0x004E0AE4 is a bare `ret`, CTCGatherPointBuilding overrides it).
// Extent: the catalog row ran on into slot 23 (0x004E0AE1), slot 29 (0x004E0AE4) and slot 30 (0x004E0AE5); see SPLITS.tsv.
#include "fable_compiler.h"
#pragma optimize("s", on)
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_STLP_VECTOR
#include "engine/CTCGatherPointBase.h"

void CTCGatherPointBase::OnInitialActivate()
{
    Init();   // virtual: slot 29
}