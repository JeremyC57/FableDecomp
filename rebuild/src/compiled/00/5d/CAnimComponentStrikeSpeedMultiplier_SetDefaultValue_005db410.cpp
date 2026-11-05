// Leaf: ?SetDefaultValue@CAnimComponentStrikeSpeedMultiplier@@QAEXXZ
struct CAnimComponentStrikeSpeedMultiplier { char pad[0x8]; unsigned long f; void SetDefaultValue(); };
void CAnimComponentStrikeSpeedMultiplier::SetDefaultValue(){ this->f = 0x3f800000u; }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?SetDefaultValue@CAnimComponentStrikeSpeedMultiplier@@UAEXXZ); evidence: RTTI vtable slot + PDB name set.
