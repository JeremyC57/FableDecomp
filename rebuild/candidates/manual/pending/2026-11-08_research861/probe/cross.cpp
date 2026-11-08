// Which cross-product spelling reproduces retail 0x00986FAC's load orders?
// retail (eax = &orientation; Up +0, Forward +0xc):  X: fld f.Y fmul u.Z ; fld u.Y fmul f.Z ; fsubp
//                                                     Y: fld f.Z fmul u.X ; fld f.X fmul u.Z ; fsubp
//                                                     Z: fld u.Y fmul f.X ; fld f.Y fmul u.X ; fsubp
struct V { float X, Y, Z; V() {} V(float x, float y, float z) : X(x), Y(y), Z(z) {}
    V CrossProduct(const V& b) const { return V(Y * b.Z - Z * b.Y, Z * b.X - X * b.Z, X * b.Y - Y * b.X); }
    V CrossProduct2(const V& b) const { return V(b.Z * Y - b.Y * Z, b.X * Z - b.Z * X, b.Y * X - b.X * Y); } };
struct S { V Up; V Forward; };
__forceinline V CrossFree(const V& a, const V& b) { return V(a.Y * b.Z - a.Z * b.Y, a.Z * b.X - a.X * b.Z, a.X * b.Y - a.Y * b.X); }
struct O { virtual const S& Get() const; };
extern O* gO;
void c1_member_fwd_up(V& out) { const S& o = gO->Get(); out = o.Forward.CrossProduct(o.Up); }
void c2_member_up_fwd(V& out) { const S& o = gO->Get(); out = o.Up.CrossProduct(o.Forward); }
void c3_free_fwd_up(V& out) { const S& o = gO->Get(); out = CrossFree(o.Forward, o.Up); }
void c4_free_up_fwd(V& out) { const S& o = gO->Get(); out = CrossFree(o.Up, o.Forward); }
void c5_member2_fwd_up(V& out) { const S& o = gO->Get(); out = o.Forward.CrossProduct2(o.Up); }
void c6_inline_expr(V& out) { const S& o = gO->Get(); const V& u = o.Up; const V& f = o.Forward;
    out = V(f.Y * u.Z - f.Z * u.Y, f.Z * u.X - f.X * u.Z, f.X * u.Y - f.Y * u.X); }
void c7_refs_member(V& out) { const S& o = gO->Get(); const V& u = o.Up; const V& f = o.Forward; out = f.CrossProduct(u); }
