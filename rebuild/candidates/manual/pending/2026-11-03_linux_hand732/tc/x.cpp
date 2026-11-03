extern const float D;
struct V { float X,Y,Z; float M() const { return (X*X+Z*Z+Y*Y); } void N(); };
void f1(V& v){ if (v.M() > D*D) v.N(); }
void f2(V& v){ if (!(v.M() < D*D)) v.N(); }
void f3(V& v){ if (v.M() >= D*D) v.N(); }
void f4(V& v){ if (D*D < v.M()) v.N(); }
void f5(V& v){ if (D*D <= v.M()) v.N(); }
void f6(V& v){ if (!(D*D > v.M())) v.N(); }
