struct V { int x,y,z; };
struct P { char a[0xc]; V vc; V v18; unsigned long f24,f28; };
struct C { char a[0xc]; unsigned long fc; char b[0x48-0x10]; V v48; V v54; unsigned long f60,f64; void U(P const& primitive, unsigned long k); };
void C::U(P const& primitive, unsigned long k){ this->fc=k; this->v48=primitive.vc; this->v54=primitive.v18; this->f60=primitive.f28; this->f64=primitive.f24; }