struct V { float X, Y, Z; };
extern V gA, gB; extern V* gpA; extern V* gpB; extern float gF;
struct C { V* p; V* q; V m; float e1() const; float e2() const; float e3(const V& b) const; float e4(const V& b) const; float e5(const V& b) const; };
float C::e1() const { return p->X * q->X; }        // two pointer members, equal disp: p (+0) vs q (+4) base load order
float C::e2() const { return q->X * p->X; }
float C::e3(const V& b) const { return b.X * m.X; } // param(+0) vs this(+0), text reversed from g2
float C::e4(const V& b) const { return p->X * b.X; } // pointer member vs param, equal disp
float C::e5(const V& b) const { return b.X * p->X; }
float k1_globptr() { return gpA->X * gpB->X; }       // two global pointers, equal disp
float k2_globptr_rev() { return gpB->X * gpA->X; }
float k3_locptr(const V& a) { V* pa = gpA; V* pb = gpB; return pa->X * pb->X; }
float k4_locptr_rev(const V& a) { V* pa = gpA; V* pb = gpB; return pb->X * pa->X; }
float k5_byval_ind(V a, const V& b) { return a.X * b.X; }      // frame direct (a.X at esp+4) vs indirect b.X
float k6_byval_ind_rev(V a, const V& b) { return b.X * a.X; }
float k7_byval_ind2(V a, const V& b) { return a.Z * b.X; }
float k8_glob_ind(const V& a) { return gA.X * a.Z; }           // global (direct) vs indirect with larger disp
float k9_reg_operand(const V& a, const V& b, float s) { return (a.X + s) * b.Y; }   // left is an x87 register value
float k10_reg_operand_rev(const V& a, const V& b, float s) { return b.Y * (a.X + s); }
float k11_reg_both(const V& a, const V& b, float s, float t) { return (a.X + s) * (b.Y + t); }
float k12_reg_both_rev(const V& a, const V& b, float s, float t) { return (b.Y + t) * (a.X + s); }
float k13_div(const V& a, const V& b) { return a.X / b.Y; }       // non-commutative
float k14_div_rev(const V& a, const V& b) { return b.Y / a.X; }
float k15_sub_rev(const V& a, const V& b) { return b.Y - a.X; }
float k16_disp_eq_param2first(const V& a, const V& b) { gF = b.X; return a.X * b.X; }  // b used first: does it matter now (equal disp)?
float k17_preoff(const V& a, const V& b) { const float* pb = &b.Y; return a.X * pb[0]; }  // pb pre-offset: disp 0 for b.Y
float k18_preoff2(const V& a, const V& b) { const float* pb = &b.Y; return pb[0] * a.X; }
float k19_neg_disp(const V& a, const V& b) { const float* pb = &b.Z; return a.X * pb[-1]; }  // pb[-1] = b.Y with disp -4
float k20_sum_products(const V& a, const V& b) { return a.X * b.X + a.Y * b.Y; }   // two-term: which term first
float k21_sum_products_rev(const V& a, const V& b) { return a.Y * b.Y + a.X * b.X; }
float k22_sum_split(const V& a, const V& b) { float t = a.X * b.X; return t + a.Y * b.Y; }
