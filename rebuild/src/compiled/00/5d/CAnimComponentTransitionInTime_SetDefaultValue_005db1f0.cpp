// Leaf: ?SetDefaultValue@CAnimComponentTransitionInTime@@UAEXXZ
// Switch route (NF318, 2026-11-23): defined on the real class of engine/CAnimComponentTransitionInTime.h (class-model switch FABLE_RETAIL_ANIMCOMPONENT; was ?SetDefaultValue@CAnimComponentTransitionInTime_Methods@@QAEXXZ);
//   the retail RTTI vftable placement (ghidra_out/labels_rtti_port.tsv) names this PDB member at this address. Bytes unchanged.
// Preamble: the define / include block of the landed precedent CAnimComponentFlags_TransferBinaryOut_005db1e0.cpp.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_ANIMCOMPONENT
#include "fable_string.h"
#include "engine/CDataOutputStream.h"
#include "engine/CAnimComponentFlags.h"
#include "engine/CAnimComponentTransitionInTime.h"  // retyped onto the PDB layout; byte parity re-verified
void CAnimComponentTransitionInTime::SetDefaultValue(){ this->TransitionInTime = 3u; }