// Leaf: ?Transfer@CSkeletalMorphDef@@QAEXPAVCPersistContext@@@Z
class CPersistContext { public: void Xfer_004657f2(void* sub); };
struct CSkeletalMorphDef { char pad[0x28]; void Transfer(CPersistContext* ctx); };
void CSkeletalMorphDef::Transfer(CPersistContext* ctx){ ctx->Xfer_004657f2(reinterpret_cast<char*>(this) + 0x28); }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?Transfer@CSkeletalMorphDef@@UAEXAAVCPersistContext@@@Z); evidence: RTTI vtable slot + PDB name set.
