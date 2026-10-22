struct V { int x,y,z; };
struct V6 { int m[6]; };
struct P { char a[0xc]; V vc; unsigned long f18,f1c,f20,f24; V6 blk28; };
struct C { char a[0xc]; unsigned long fc; char b[0x48-0x10]; V v48; unsigned long f54,f58,f5c,f60; V6 blk64; void U(P const& primitive, unsigned long k); };
void C::U(P const& primitive, unsigned long k){ this->fc=k; this->v48=primitive.vc; this->f54=primitive.f18; this->f58=primitive.f1c; this->f5c=primitive.f20; this->f60=primitive.f24; this->blk64=primitive.blk28; }