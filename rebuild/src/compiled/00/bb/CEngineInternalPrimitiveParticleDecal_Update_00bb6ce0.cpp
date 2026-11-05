// Leaf: ?Update@CEngineInternalPrimitiveParticleDecal@@QAEXABUP@@K@Z
struct V { int x,y,z; };
struct V6 { int m[6]; };
struct P { char a[0xc]; V vc; unsigned long f18,f1c,f20,f24; V6 blk28; };
struct CEngineInternalPrimitiveParticleDecal { char a[0xc]; unsigned long fc; char b[0x48-0x10]; V v48; unsigned long f54,f58,f5c,f60; V6 blk64; void Update(P const& primitive, unsigned long k); };
void CEngineInternalPrimitiveParticleDecal::Update(P const& primitive, unsigned long k){ this->fc=k; this->v48=primitive.vc; this->f54=primitive.f18; this->f58=primitive.f1c; this->f5c=primitive.f20; this->f60=primitive.f24; this->blk64=primitive.blk28; }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?Update@CEngineInternalPrimitiveParticleDecal@@UAEXABVCEnginePrimitive@@K@Z); evidence: RTTI vtable slot + PDB name set.
