// Layout: retail STLport 4.6 header offsets; member names re-bound by the bytes this body touches: `CameraResetToViewBehindHeroCount` -> `CreateCreatureDelayFrames` (+0x4c).
// Leaf: ?SetCreatureCreationDelayFrames@CGameScriptInterface@@UAEXJ@Z
// Signature port (NF320, 2026-11-23): defined on the real class of engine/CGameScriptInterface.h under FABLE_RETAIL_NSCRIPT with the PDB parameter / return types (was ?SetCreatureCreationDelayFrames@CGameScriptInterface_Methods@@QAEXH@Z);
//   the retail RTTI vftable placement (ghidra_out/labels_rtti_port.tsv) names this PDB member here.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
void CGameScriptInterface::SetCreatureCreationDelayFrames(long frames) {
    this->CreateCreatureDelayFrames = frames;
}
