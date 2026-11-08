struct V { float X, Y, Z; };
struct W { float P[8]; };
extern V gA, gB;
struct C { V m; float k; float g1(const V& b) const; float g2(const V& b) const; float g3(const V& b) const; };
float h1_eqoff_ab(const V& a, const V& b) { return a.X * b.X; }          // equal offsets, params (a,b)
float h2_eqoff_ba(const V& b, const V& a) { return a.X * b.X; }          // equal offsets, params (b,a)
float h3_eqoff_ba_t(const V& b, const V& a) { return b.X * a.X; }
float h4_xz(const V& a) { return a.X * a.Z; }
float h5_zx(const V& a) { return a.Z * a.X; }
float h6_yz(const V& a) { return a.Y * a.Z; }
float h7_aY_bX(const V& a, const V& b) { return a.Y * b.X; }             // a higher offset, a first param
float h8_aX_bY_rev(const V& b, const V& a) { return a.Y * b.X; }         // a higher offset, a second param
float C::g1(const V& b) const { return m.Y * b.X; }                     // this+4 vs param+0
float C::g2(const V& b) const { return m.X * b.X; }                     // this+0 vs param+0
float C::g3(const V& b) const { return b.Y * m.Z; }                     // param+4 vs this+8
float h9_arr(const W& w) { return w.P[1] * w.P[5]; }
float h10_arr_rev(const W& w) { return w.P[5] * w.P[1]; }
float h11_ptrs(const float* p, const float* q) { return p[0] * q[0]; }
float h12_ptrs_rev(const float* p, const float* q) { return q[0] * p[0]; }
float h13_ptroff(const float* p, const float* q) { return p[2] * q[0]; }
float h14_ptroff_rev(const float* p, const float* q) { return q[0] * p[2]; }
float h15_ptroff2(const float* p, const float* q) { return p[0] * q[2]; }
float h16_loc(const V& a, const V& b) { V la = a; V lb = b; return la.X * lb.Y; }   // locals (copies) -- may fold
float h17_byval(V a, V b) { return a.X * b.Y; }                                      // by-value params (stack)
float h18_byval_rev(V a, V b) { return a.Y * b.X; }
float h19_byval_eq(V a, V b) { return a.X * b.X; }
float h20_byval_eq_t(V a, V b) { return b.X * a.X; }
float h21_glob_same(void) { return gA.X * gA.Y; }
float h22_glob_same_rev(void) { return gA.Y * gA.X; }
float h23_add(const V& a, const V& b) { return a.X + b.Y; }           // addition: same rule?
float h24_add_rev(const V& a, const V& b) { return b.Y + a.X; }
float h25_add_eq(const V& a, const V& b) { return a.X + b.X; }
float h26_add_eq_t(const V& a, const V& b) { return b.X + a.X; }
float h27_sub(const V& a, const V& b) { return a.X - b.Y; }           // non-commutative: must keep order
float h28_mul3(const V& a, const V& b) { return a.X * b.Y * a.Z; }
float h29_mul3b(const V& a, const V& b) { return a.Z * b.Y * a.X; }
