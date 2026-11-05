// Leaf: ?Update@CEngineInternalPrimitive2DText@@QAEXAAUP@@K@Z
struct V { int x,y,z; };
struct P { char a[0xc]; V vc; int f18,f1c,f20,f24,f28; };
struct Sub { void M(void* p); };
struct CEngineInternalPrimitive2DText { char a[0xc]; unsigned long fc; char b[0x48-0x10]; V v48; int f54,f58; char c[0x60-0x5c]; Sub sub60; int f64,f68; void Update(P& p, unsigned long k); };
void CEngineInternalPrimitive2DText::Update(P& p, unsigned long k){ this->fc=k; this->v48=p.vc; this->f54=p.f1c; this->f58=p.f20; this->f68=p.f18; this->sub60.M(&p.f24); this->f64=p.f28; }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?Update@CEngineInternalPrimitive2DText@@UAEXABVCEnginePrimitive@@K@Z); evidence: RTTI vtable slot + PDB name set.
