struct V3 { float x,y,z; };
struct CM { char pad[0x28]; float p; long q; const float GetP() const { return p; } float GetPd() const { return (float)p; } };
struct T { const V3& GetPos() const; CM& GetCM(); };
struct N { bool Nav(const V3&, const V3&, float) const; };
struct A { T* t; T* u; N* n; bool f1(); bool f2(); };
bool A::f1() { return n->Nav(t->GetPos(), u->GetPos(), t->GetCM().GetP()); }
bool A::f2() { return n->Nav(t->GetPos(), u->GetPos(), t->GetCM().GetPd()); }
