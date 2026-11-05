// Leaf: ?Transfer@CReplaceableMeshDef@@QAEXPAVCPersistContext@@@Z
class CPersistContext { public: void Xfer_004ea21e(void* sub); };
struct CReplaceableMeshDef { char pad[0x28]; void Transfer(CPersistContext* ctx); };
void CReplaceableMeshDef::Transfer(CPersistContext* ctx){ ctx->Xfer_004ea21e(reinterpret_cast<char*>(this) + 0x28); }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?Transfer@CReplaceableMeshDef@@UAEXAAVCPersistContext@@@Z); evidence: RTTI vtable slot + PDB name set.
