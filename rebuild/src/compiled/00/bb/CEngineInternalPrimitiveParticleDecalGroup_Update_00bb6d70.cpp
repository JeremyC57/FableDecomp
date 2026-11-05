// Leaf: ?Update@CEngineInternalPrimitiveParticleDecalGroup@@QAEXABUP@@K@Z
struct P { char a[0xc]; unsigned long fc; unsigned short f10; char _[2]; unsigned long f14,f18,f1c; unsigned char f20; };
struct CEngineInternalPrimitiveParticleDecalGroup { char a[0xc]; unsigned long fc; char b[0xac-0x10]; unsigned long fac; unsigned short fb0; char c[2]; unsigned long fb4,fb8,fbc; unsigned char fc0; void Update(P const& primitive, unsigned long k); };
void CEngineInternalPrimitiveParticleDecalGroup::Update(P const& primitive, unsigned long k){ this->fc=k; this->fac=primitive.fc; this->fb0=primitive.f10; this->fb4=primitive.f14; this->fb8=primitive.f18; this->fbc=primitive.f1c; this->fc0=primitive.f20; }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?Update@CEngineInternalPrimitiveParticleDecalGroup@@UAEXABVCEnginePrimitive@@K@Z); evidence: RTTI vtable slot + PDB name set.
