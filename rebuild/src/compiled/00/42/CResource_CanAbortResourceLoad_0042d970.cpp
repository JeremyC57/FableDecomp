// Constant-return member: returns false. __fastcall this=ecx.
// Leaf: ?CanAbortResourceLoad@CResource@@UBE_NJ@Z
// Signature port (NF320, 2026-11-23): defined on the real class of engine/CResource.h under FABLE_RETAIL_DEF_CLASS_BASE with the PDB parameter / return types (was ?CanAbortResourceLoad@CResource_Methods@@QAE_NH@Z);
//   the retail RTTI vftable placement (ghidra_out/labels_rtti_port.tsv) names this PDB member here.
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"
#include "engine/CResource.h"
bool CResource::CanAbortResourceLoad(long a0) const { return false; }
