struct V3 { float x,y,z; };
struct CM { char pad[0x28]; float p; long q; float GetP() const { return p; } };
struct T { const V3& GetPos() const; CM& GetCM(); CM* GetPCM(); };
struct N { bool Nav(const V3&, const V3&, float) const; };
struct A { T* t; T* u; N* n; CM* pcm; bool f1(); bool f2(); bool f3(); bool f4(); };
bool A::f1() { return n->Nav(t->GetPos(), u->GetPos(), pcm->GetP()); }
bool A::f2() { return n->Nav(t->GetPos(), u->GetPos(), t->GetPCM()->GetP()); }
bool A::f3() { CM& c = t->GetCM(); return n->Nav(t->GetPos(), u->GetPos(), c.GetP()); }
bool A::f4() { return n->Nav(t->GetPos(), u->GetPos(), t->GetCM().p); }
