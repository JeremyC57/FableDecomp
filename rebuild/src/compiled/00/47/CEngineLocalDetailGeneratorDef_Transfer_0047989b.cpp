// Leaf: ?Transfer@CEngineLocalDetailGeneratorDef@@QAEXPAVCPersistContext@@@Z
class CPersistContext { public: void Xfer_004798ab(void* sub); };
struct CEngineLocalDetailGeneratorDef { char pad[0x3c]; void Transfer(CPersistContext* ctx); };
void CEngineLocalDetailGeneratorDef::Transfer(CPersistContext* ctx){ ctx->Xfer_004798ab(reinterpret_cast<char*>(this) + 0x3c); }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?Transfer@CEngineLocalDetailGeneratorDef@@UAEXAAVCPersistContext@@@Z); evidence: RTTI vtable slot + PDB name set.
