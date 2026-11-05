// Leaf: ?Transfer@CHitLocationsDef@@QAEXPAVCPersistContext@@@Z
class CPersistContext { public: void Xfer_00466a47(void* sub); };
struct CHitLocationsDef { char pad[0x28]; void Transfer(CPersistContext* ctx); };
void CHitLocationsDef::Transfer(CPersistContext* ctx){ ctx->Xfer_00466a47(reinterpret_cast<char*>(this) + 0x28); }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?Transfer@CHitLocationsDef@@UAEXAAVCPersistContext@@@Z); evidence: RTTI vtable slot + PDB name set.
