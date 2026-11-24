struct V3 { float x,y,z; };
struct CM { char pad[0x28]; float p; long q; float GetP() const { return p; } };
struct T { const V3& GetPos() const; CM& GetCM(); };
struct N { bool Nav(const V3&, const V3&, float) const; void W(float); };
struct A { T* t; T* u; N* n; CM* pcm; CM cm; bool f1(); void f2(); void f3(); void f4(); bool f5(); };
bool A::f1() { return n->Nav(t->GetPos(), u->GetPos(), cm.GetP()); }
void A::f2() { n->W(cm.GetP()); }
void A::f3() { n->W(pcm->GetP()); }
void A::f4() { n->W(t->GetCM().GetP()); }
bool A::f5() { CM* c = &t->GetCM(); return n->Nav(t->GetPos(), u->GetPos(), c->GetP()); }
