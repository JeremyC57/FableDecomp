// Leaf: ?Transfer@CIdleSchedulerDef@@QAEXPAVCPersistContext@@@Z
class CPersistContext { public: void Xfer_004e6258(void* sub); };
struct CIdleSchedulerDef { char pad[0x3c]; void Transfer(CPersistContext* ctx); };
void CIdleSchedulerDef::Transfer(CPersistContext* ctx){ ctx->Xfer_004e6258(reinterpret_cast<char*>(this) + 0x3c); }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?Transfer@CIdleSchedulerDef@@UAEXAAVCPersistContext@@@Z); evidence: RTTI vtable slot + PDB name set.
