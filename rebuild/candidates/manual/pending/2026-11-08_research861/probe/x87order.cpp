// x87 operand-order probe: which operand of a*b becomes the fld (first) and which the fmul (second)?
struct V { float X, Y, Z; };
extern V gA, gB; extern float gF;
struct C { V m; float k; float f_mem_param(const V& b) const; float f_param_mem(const V& b) const; };
float f1_ab(const V& a, const V& b) { return a.X * b.Y; }              // a from param 1, b from param 2
float f2_ba(const V& a, const V& b) { return b.Y * a.X; }              // textual swap
float f3_ab_rev(const V& b, const V& a) { return a.X * b.Y; }          // params declared in the other order
float f4_same(const V& a) { return a.X * a.Y; }                        // same base, X and Y
float f5_same_rev(const V& a) { return a.Y * a.X; }
float f6_const(const V& a) { return a.X * 2.5f; }
float f7_const_rev(const V& a) { return 2.5f * a.X; }
float f8_glob(const V& a) { return a.X * gB.Y; }                       // param vs global
float f9_glob_rev(const V& a) { return gB.Y * a.X; }
float f10_glob2() { return gA.X * gB.Y; }                             // two globals
float f11_glob2_rev() { return gB.Y * gA.X; }
float f12_ptr(const V* a, const V& b) { return a->X * b.Y; }          // pointer param vs ref param
float C::f_mem_param(const V& b) const { return m.X * b.Y; }          // this member vs param
float C::f_param_mem(const V& b) const { return b.Y * m.X; }
float f13_cross(const V& a, const V& b) { return a.X * b.Y - a.Y * b.X; }     // one cross term
float f14_cross_ba(const V& a, const V& b) { return b.Y * a.X - b.X * a.Y; }
float f15_dot(const V& a, const V& b) { return a.X * b.X + a.Y * b.Y + a.Z * b.Z; }
float f16_dot_ba(const V& a, const V& b) { return b.X * a.X + b.Y * a.Y + b.Z * a.Z; }
float f17_local(const V& a, const V& b) { float t = b.Y; return a.X * t; }    // named local of b
float f18_local2(const V& a, const V& b) { float t = a.X; return t * b.Y; }
float f19_cast(const V& a, const V& b) { return (float)(double)a.X * b.Y; }
float f20_idx(const V& a, const V& b) { const float* pb = &b.X; return a.X * pb[1]; }
float f21_first_use_b(const V& a, const V& b) { gF = b.Z; return a.X * b.Y; }  // b touched first in the function
float f22_first_use_a(const V& a, const V& b) { gF = a.Z; return a.X * b.Y; }
