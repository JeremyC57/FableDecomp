// Leaf: ?Update@CEngineInternalPrimitiveParticleSprite@@QAEXABUP@@K@Z
struct V { int x,y,z; };
struct P { char a[0xc]; unsigned long fc; V v10; unsigned long f1c,f20,f24,f28; };
struct CEngineInternalPrimitiveParticleSprite { char a[0xc]; unsigned long fc; char b[0x48-0x10]; V v48; unsigned long f54,f58,f5c,f60,f64; void Update(P const& primitive, unsigned long k); };
void CEngineInternalPrimitiveParticleSprite::Update(P const& primitive, unsigned long k){ this->fc=k; this->v48=primitive.v10; this->f54=primitive.f1c; this->f58=primitive.f20; this->f5c=primitive.f24; this->f60=primitive.f28; this->f64=primitive.fc; }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?Update@CEngineInternalPrimitiveParticleSprite@@UAEXABVCEnginePrimitive@@K@Z); evidence: RTTI vtable slot + PDB name set.
