// Leaf: ?Transfer@CCreatureAbilityDef@@QAEXPAVCPersistContext@@@Z
class CPersistContext { public: void Xfer_00459363(void* sub); };
struct CCreatureAbilityDef { char pad[0x3c]; void Transfer(CPersistContext* ctx); };
void CCreatureAbilityDef::Transfer(CPersistContext* ctx){ ctx->Xfer_00459363(reinterpret_cast<char*>(this) + 0x3c); }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?Transfer@CCreatureAbilityDef@@UAEXAAVCPersistContext@@@Z); evidence: RTTI vtable slot + PDB name set.
