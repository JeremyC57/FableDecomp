// Leaf: ?Transfer@CSimVoicesDef@@QAEXPAVCPersistContext@@@Z
class CPersistContext { public: void Xfer_00464b9a(void* sub); };
struct CSimVoicesDef { char pad[0x3c]; void Transfer(CPersistContext* ctx); };
void CSimVoicesDef::Transfer(CPersistContext* ctx){ ctx->Xfer_00464b9a(reinterpret_cast<char*>(this) + 0x3c); }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?Transfer@CSimVoicesDef@@UAEXAAVCPersistContext@@@Z); evidence: RTTI vtable slot + PDB name set.
