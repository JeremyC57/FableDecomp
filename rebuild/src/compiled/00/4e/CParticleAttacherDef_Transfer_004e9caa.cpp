// Leaf: ?Transfer@CParticleAttacherDef@@QAEXPAVCPersistContext@@@Z
class CPersistContext { public: void Xfer_004e9cba(void* sub); };
struct CParticleAttacherDef { char pad[0x28]; void Transfer(CPersistContext* ctx); };
void CParticleAttacherDef::Transfer(CPersistContext* ctx){ ctx->Xfer_004e9cba(reinterpret_cast<char*>(this) + 0x28); }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?Transfer@CParticleAttacherDef@@UAEXAAVCPersistContext@@@Z); evidence: RTTI vtable slot + PDB name set.
