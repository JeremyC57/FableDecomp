// Leaf: ?Transfer@CCardPositionsDef@@QAEXPAVCPersistContext@@@Z
class CPersistContext { public: void Xfer_00475d99(void* sub); };
struct CCardPositionsDef { char pad[0x3c]; void Transfer(CPersistContext* ctx); };
void CCardPositionsDef::Transfer(CPersistContext* ctx){ ctx->Xfer_00475d99(reinterpret_cast<char*>(this) + 0x3c); }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?Transfer@CCardPositionsDef@@UAEXAAVCPersistContext@@@Z); evidence: RTTI vtable slot + PDB name set.
