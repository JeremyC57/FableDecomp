// Leaf: ?IsPreloaded@CEngine@@UBE_NXZ
// Header route (NF319, 2026-11-23): defined on the real class of engine/CEngine.h (was ?IsPreloaded@CEngine_Methods@@QAE_NXZ); the retail RTTI vftable
//   placement (ghidra_out/labels_rtti_port.tsv) names this PDB member at this address;
//   declared there under FABLE_RETAIL_CENGINE + FABLE_RETAIL_CENGINE_OVERRIDES (switches landed rows define). Bytes unchanged.
#define FABLE_RETAIL_CIENGINE_RENDER
#define FABLE_RETAIL_CENGINE
#define FABLE_RETAIL_CENGINE_OVERRIDES
// CEngine_Methods::IsPreloaded @ 0x00b23a70
#include "engine/CEngine.h"  // retyped onto the PDB layout; byte parity re-verified
#include "rebuild_abi.h"
bool CEngine::IsPreloaded() const {
    if (NewSceneFlag || CutscenePreloadFlag) return false;
    return true;
}