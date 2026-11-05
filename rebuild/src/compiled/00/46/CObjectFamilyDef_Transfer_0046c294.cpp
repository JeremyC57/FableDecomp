// Leaf: ?Transfer@CObjectFamilyDef@@QAEXPAVCPersistContext@@@Z
class CPersistContext { public: void Xfer_0046c2a4(void* sub); };
struct CObjectFamilyDef { char pad[0x3c]; void Transfer(CPersistContext* ctx); };
void CObjectFamilyDef::Transfer(CPersistContext* ctx){ ctx->Xfer_0046c2a4(reinterpret_cast<char*>(this) + 0x3c); }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?Transfer@CObjectFamilyDef@@UAEXAAVCPersistContext@@@Z); evidence: RTTI vtable slot + PDB name set.
