extern const float D;
struct V { float X,Y,Z; V(){} V(float x,float y,float z):X(x),Y(y),Z(z){} V(const V& o):X(o.X),Y(o.Y),Z(o.Z){}
 __forceinline float M() const { return X*X+Z*Z+Y*Y; }
 __forceinline float M2() const { return X*X+Y*Y; }
 void N(); };
struct T { int pad[4]; V L; void g(V&); };
void f1(T* t, V& d){ d.N(); if (t->L.M() > D*D) { d.N(); } }
void f2(T* t, V& d){ V l = t->L; d.N(); if (l.M() > D*D) { d = l; d.N(); } }
void f3(const V& a, V& d){ V l(a.X, a.Y, 0.0f); if (l.M() > D*D) { d = l; d.N(); } }
void f4(const V& a, V& d){ V l = a; l.Z = 0.0f; if (l.M() > D*D) { l.N(); d = l; } }
