// Leaf: ?Transfer@CTextureReplacementDef@@QAEXPAVCPersistContext@@@Z
class CPersistContext { public: void Xfer_00469c3e(void* sub); };
struct CTextureReplacementDef { char pad[0x28]; void Transfer(CPersistContext* ctx); };
void CTextureReplacementDef::Transfer(CPersistContext* ctx){ ctx->Xfer_00469c3e(reinterpret_cast<char*>(this) + 0x28); }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?Transfer@CTextureReplacementDef@@UAEXAAVCPersistContext@@@Z); evidence: RTTI vtable slot + PDB name set.
