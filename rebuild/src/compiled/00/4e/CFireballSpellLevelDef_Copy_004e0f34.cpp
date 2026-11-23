// Leaf: ?Copy@CFireballSpellLevelDef@@UAEXPBVCDefClassBase@@@Z
// Header route (NF319, 2026-11-23): defined on the real class of engine/CFireballSpellLevelDef.h (was ?Copy@CFireballSpellLevelDef_Methods@@QAEXPBVCDefClassBase@@@Z); the retail RTTI vftable
//   placement (ghidra_out/labels_rtti_port.tsv) names this PDB member at this address; declared by the add-only opt-in
//   FABLE_RETAIL_CFIREBALLSPELLLEVELDEF_RTTI_DECLS. Bytes unchanged.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CFIREBALLSPELLLEVELDEF
#define FABLE_RETAIL_CFIREBALLSPELLLEVELDEF_RTTI_DECLS
#include "engine/CFireballSpellLevelDef.h"
#include "engine/CDefClassBase.h"                       // retail def chain (0x25 packed, vtable 0x01230BCC)



void CFireballSpellLevelDef::Copy(const CDefClassBase* src) {
    ((CDefClassBase*)(this))->operator=(*(const CDefClassBase*)(src));
    this->SpellLevel = *(unsigned long const*)((char const*)src + 0x28);
}
