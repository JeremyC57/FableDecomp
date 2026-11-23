// CExpressionSubDef::Transfer(CPersistContext&) -- transfers the one long member at +0x28 (ExpressionDef) through the context
// Leaf: ?Transfer@CExpressionSubDef@@UAEXAAVCPersistContext@@@Z
// Signature port (NF320, 2026-11-23): defined on the real class of engine/CExpressionSubDef.h (class-model switch FABLE_RETAIL_CEXPRESSIONSUBDEF, landed definers exist) with the
//   PDB parameter type (was ?Transfer@CExpressionSubDef_Methods@@QAEXPAVCPersistContext@@@Z); the override is declared by the add-only opt-in FABLE_RETAIL_CEXPRESSIONSUBDEF_TRANSFER_DECL (this batch's hdr/,
//   equiv_check: 0 differing landed rows). The retail RTTI vftable placement (ghidra_out/labels_rtti_port.tsv) names this PDB member here.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CEXPRESSIONSUBDEF
#define FABLE_RETAIL_CEXPRESSIONSUBDEF_TRANSFER_DECL
#include "engine/CExpressionSubDef.h"
class CPersistContext { public: template <class T> void Transfer(T& value); };   // HEAD's local model, kept: the ??$Transfer@J binding is unchanged
void CExpressionSubDef::Transfer(CPersistContext& ctx){ ctx.Transfer(ExpressionDef); }
