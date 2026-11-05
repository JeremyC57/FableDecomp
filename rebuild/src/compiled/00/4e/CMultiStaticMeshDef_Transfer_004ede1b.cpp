// Leaf: ?Transfer@CMultiStaticMeshDef@@QAEXPAVCPersistContext@@@Z
class CPersistContext { public: void Xfer_004ede2b(void* sub); };
struct CMultiStaticMeshDef { char pad[0x28]; void Transfer(CPersistContext* ctx); };
void CMultiStaticMeshDef::Transfer(CPersistContext* ctx){ ctx->Xfer_004ede2b(reinterpret_cast<char*>(this) + 0x28); }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?Transfer@CMultiStaticMeshDef@@UAEXAAVCPersistContext@@@Z); evidence: RTTI vtable slot + PDB name set.
