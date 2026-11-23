// Member setter at this+0x25: one stack arg, `ret 4`. __fastcall this=ecx.
// Leaf: ?SetInstantiationName@CParentDefClassBase@@UAEXVCDefString@@@Z
// Signature port (NF320, 2026-11-23): defined on the real class of engine/CParentDefClassBase.h under FABLE_RETAIL_DEF_CLASS_BASE with
//   FABLE_PDCB_TYPED_MEMBERS (the typed CDefString member at +0x25) and the PDB parameter type (was ?SetInstantiationName@CParentDefClassBase_Methods@@QAEXH@Z);
//   the retail RTTI vftable placement (ghidra_out/labels_rtti_port.tsv) names this PDB member here.
// Preamble: the landed precedent CScriptDef_CScriptDef_00f29530.cpp (typed CParentDefClassBase members).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_PDCB_TYPED_MEMBERS
#define FABLE_STLPORT_HTTP
#include "fable_string.h"
#include "engine/CDefString.h"
#include "engine/CVectorMap.h"
#include "engine/CParentDefClassBase.h"
void CParentDefClassBase::SetInstantiationName(CDefString value) { InstantiationName = value; }
