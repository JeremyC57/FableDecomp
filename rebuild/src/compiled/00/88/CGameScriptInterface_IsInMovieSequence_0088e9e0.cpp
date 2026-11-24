// Leaf: ?IsInMovieSequence@CGameScriptInterface@@UBE_NXZ
// Switch route (NF318, 2026-11-23): defined on the real class of engine/CGameScriptInterface.h (class-model switch FABLE_RETAIL_NSCRIPT; was ?IsInMovieSequence@CGameScriptInterface_Methods@@QAEEXZ);
//   the retail RTTI vftable placement (ghidra_out/labels_rtti_port.tsv) names this PDB member at this address. Bytes unchanged.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
bool CGameScriptInterface::IsInMovieSequence() const { return InMovieSequence; }
