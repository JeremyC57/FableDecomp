// CGoldDef::Transfer(CPersistContext&) -- transfers the one long member at +0x28 (Gold) through the context
// Leaf: ?Transfer@CGoldDef@@UAEXAAVCPersistContext@@@Z
// Signature port (NF320, 2026-11-23): defined on the real class of engine/CGoldDef.h (class-model switch FABLE_RETAIL_CGOLDDEF, landed definers exist) with the
//   PDB parameter type (was ?Transfer@CGoldDef_Methods@@QAEXPAVCPersistContext@@@Z); the override is declared by the add-only opt-in FABLE_RETAIL_CGOLDDEF_TRANSFER_DECL (this batch's hdr/,
//   equiv_check: 0 differing landed rows). The retail RTTI vftable placement (ghidra_out/labels_rtti_port.tsv) names this PDB member here.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CGOLDDEF
#define FABLE_RETAIL_CGOLDDEF_TRANSFER_DECL
#include "engine/CGoldDef.h"
class CPersistContext { public: template <class T> void Transfer(T& value); };   // HEAD's local model, kept: the ??$Transfer@J binding is unchanged
void CGoldDef::Transfer(CPersistContext& ctx){ ctx.Transfer(Gold); }
