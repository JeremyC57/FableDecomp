struct V { float X, Y, Z; };
extern float gF; void sink(float);
float m1_plus_eq(const V& a, const V& b) { float r = a.X * b.X; r += a.Y * b.Y; return r; }
float m2_glob_acc(const V& a, const V& b) { gF = a.X * b.X; gF += a.Y * b.Y; return gF; }
float m3_two_stmts_store(const V& a, const V& b, float* out) { *out = a.X * b.X; *out += a.Y * b.Y; return *out; }
float m4_call_between(const V& a, const V& b) { float r = a.X * b.X; sink(r); return r + a.Y * b.Y; }
float m5_cast(const V& a, const V& b) { return (float)(double)(a.X * b.X) + a.Y * b.Y; }
float m6_paren(const V& a, const V& b) { return (a.X * b.X) + (a.Y * b.Y); }
float m7_neg(const V& a, const V& b) { return -(a.X * b.X) + a.Y * b.Y; }
float m8_minus(const V& a, const V& b) { return a.X * b.X - a.Y * b.Y; }        // non-commutative at the top: order kept?
float m9_minus_rev(const V& a, const V& b) { return a.Y * b.Y - a.X * b.X; }
float m10_three(const V& a, const V& b) { return a.X * b.Y + a.Y * b.Z + a.Z * b.X; }   // mixed displacements per product
float m11_dot_int(const V& a, const V& b) { float r = 0.0f; r = r + a.X * b.X; r = r + a.Y * b.Y; return r; }
float m12_times_one(const V& a, const V& b) { return a.X * b.X * 1.0f + a.Y * b.Y; }
float m13_ref_local(const V& a, const V& b) { const float& ax = a.X; return ax * b.X + a.Y * b.Y; }
float m14_byref_arg(const V& a, const V& b, float& r) { r = a.X * b.X; r += a.Y * b.Y; return r; }
