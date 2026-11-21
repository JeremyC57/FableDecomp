struct V3 { float x,y,z; };
struct CM { char pad[0x28]; float p; long q; __forceinline float GetP() const { return p; } float GetP2() const; float GetP3() const { float r = p; return r; } };
inline float CM::GetP2() const { return this->p; }
struct T { const V3& GetPos() const; CM& GetCM(); };
struct N { bool Nav(const V3&, const V3&, float) const; bool NavD(const V3&, const V3&, double) const; };
struct A { T* t; T* u; N* n; bool f1(); bool f2(); bool f3(); bool f4(); };
bool A::f1() { return n->Nav(t->GetPos(), u->GetPos(), t->GetCM().GetP()); }
bool A::f2() { return n->Nav(t->GetPos(), u->GetPos(), t->GetCM().GetP2()); }
bool A::f3() { return n->Nav(t->GetPos(), u->GetPos(), t->GetCM().GetP3()); }
bool A::f4() { const CM& c = t->GetCM(); return n->Nav(t->GetPos(), u->GetPos(), c.p); }
