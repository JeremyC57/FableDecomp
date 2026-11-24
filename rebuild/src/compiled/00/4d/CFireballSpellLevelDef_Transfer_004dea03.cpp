// CFireballSpellLevelDef::Transfer(CPersistContext&) -- transfers the one long member at +0x28 (SpellLevel) through the context
// Leaf: ?Transfer@CFireballSpellLevelDef@@UAEXAAVCPersistContext@@@Z
// Header route (NF323, 2026-11-24; NF320 Q3): defined on the real class of engine/CFireballSpellLevelDef.h (was ?Transfer@CFireballSpellLevelDef_Methods@@QAEXPAVCPersistContext@@@Z); the retail RTTI vftable
//   placement (ghidra_out/labels_rtti_port.tsv) names this PDB member at this address (+ Ego_r.pdb + FableWin.pdb); declared by the add-only opt-in
//   FABLE_RETAIL_CFIREBALLSPELLLEVELDEF_TRANSFER_DECL. Bytes unchanged.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CFIREBALLSPELLLEVELDEF
#define FABLE_RETAIL_CFIREBALLSPELLLEVELDEF_TRANSFER_DECL
#include "engine/CFireballSpellLevelDef.h"
class CPersistContext { public: template <class T> void Transfer(T& value); };   // HEAD's local model, kept: the ??$Transfer@J binding is unchanged
void CFireballSpellLevelDef::Transfer(CPersistContext& ctx){ ctx.Transfer(SpellLevel); }
