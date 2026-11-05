// Leaf: ?Update@CEngineInternalPrimitiveParticleLineGroup@@QAEXABUP@@K@Z
struct P { char a[0xc]; unsigned long fc; unsigned short f10; char _[2]; unsigned long f14; unsigned long f18; };
struct CEngineInternalPrimitiveParticleLineGroup { char a[0xc]; unsigned long fc; char b[0xac-0x10]; unsigned long fac; unsigned short fb0; char c[2]; unsigned long fb4; unsigned long fb8; void Update(P const& primitive, unsigned long k); };
void CEngineInternalPrimitiveParticleLineGroup::Update(P const& primitive, unsigned long k){ this->fc=k; this->fac=primitive.fc; this->fb4=primitive.f14; this->fb0=primitive.f10; this->fb8=primitive.f18; }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?Update@CEngineInternalPrimitiveParticleLineGroup@@UAEXABVCEnginePrimitive@@K@Z); evidence: RTTI vtable slot + PDB name set.
