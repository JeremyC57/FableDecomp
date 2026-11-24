struct V3 { float x,y,z; };
struct CM { char pad[0x28]; float p; long q; float GetP() const { return p; } };
struct T { const V3& GetPos() const; CM& GetCM(); };
struct N { bool Nav(const V3&, const V3&, float) const; };
struct A { T* t; T* u; N* n; bool f1(); void f2(); };
bool A::f1() { if (n->Nav(t->GetPos(), u->GetPos(), t->GetCM().GetP())) return true; return false; }
void A::f2() { n->Nav(t->GetPos(), u->GetPos(), t->GetCM().GetP()); }
