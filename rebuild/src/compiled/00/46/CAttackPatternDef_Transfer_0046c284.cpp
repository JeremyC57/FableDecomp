// Leaf: ?Transfer@CAttackPatternDef@@QAEXPAVCPersistContext@@@Z
class CPersistContext { public: void Xfer_00466a47(void* sub); };
struct CAttackPatternDef { char pad[0x3c]; void Transfer(CPersistContext* ctx); };
void CAttackPatternDef::Transfer(CPersistContext* ctx){ ctx->Xfer_00466a47(reinterpret_cast<char*>(this) + 0x3c); }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?Transfer@CAttackPatternDef@@UAEXAAVCPersistContext@@@Z); evidence: RTTI vtable slot + PDB name set.
