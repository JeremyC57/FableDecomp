// Leaf: ?Update@CEngineInternalPrimitiveParticleLine@@QAEXABUP@@K@Z
struct V { int x,y,z; };
struct P { char a[0xc]; V vc; V v18; unsigned long f24,f28; };
struct CEngineInternalPrimitiveParticleLine { char a[0xc]; unsigned long fc; char b[0x48-0x10]; V v48; V v54; unsigned long f60,f64; void Update(P const& primitive, unsigned long k); };
void CEngineInternalPrimitiveParticleLine::Update(P const& primitive, unsigned long k){ this->fc=k; this->v48=primitive.vc; this->v54=primitive.v18; this->f60=primitive.f28; this->f64=primitive.f24; }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?Update@CEngineInternalPrimitiveParticleLine@@UAEXABVCEnginePrimitive@@K@Z); evidence: RTTI vtable slot + PDB name set.
