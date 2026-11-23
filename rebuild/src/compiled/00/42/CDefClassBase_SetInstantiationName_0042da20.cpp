// Empty one-argument member: `ret 4`. __fastcall this=ecx, one stack arg.
// Leaf: ?SetInstantiationName@CDefClassBase@@UAEXVCDefString@@@Z
// Signature port (NF320, 2026-11-23): defined on the real class of engine/CDefClassBase.h under FABLE_RETAIL_DEF_CLASS_BASE with the PDB parameter / return types (was ?SetInstantiationName@CDefClassBase_Methods@@QAEXPBD@Z);
//   the retail RTTI vftable placement (ghidra_out/labels_rtti_port.tsv) names this PDB member here.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "fable_string.h"
#include "engine/CDefString.h"   // the by-value CDefString parameter needs the complete type
#include "engine/CDefClassBase.h"
void CDefClassBase::SetInstantiationName(CDefString name) {}
