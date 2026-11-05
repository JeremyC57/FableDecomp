// Leaf: ?Transfer@CHeroSuitDef@@QAEXPAVCPersistContext@@@Z
class CPersistContext { public: void Xfer_004e5601(void* sub); };
struct CHeroSuitDef { char pad[0x28]; void Transfer(CPersistContext* ctx); };
void CHeroSuitDef::Transfer(CPersistContext* ctx){ ctx->Xfer_004e5601(reinterpret_cast<char*>(this) + 0x28); }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?Transfer@CHeroSuitDef@@UAEXAAVCPersistContext@@@Z); evidence: RTTI vtable slot + PDB name set.
