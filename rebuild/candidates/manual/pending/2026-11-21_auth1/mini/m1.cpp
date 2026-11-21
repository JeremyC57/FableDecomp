struct V3 { float x,y,z; };
struct CM { char pad[0x28]; float p; long q; float GetP() const { return p; } long GetQ() const { return q; } };
struct T { const V3& GetPos() const; CM& GetCM(); };
struct IP { T* GetPItem() const; };
struct N { bool Nav(const V3&, const V3&, float) const; bool NavL(const V3&, const V3&, long) const; };
struct A { T* t; IP e; N* n; bool f1(); bool f2(); bool f3(); };
bool A::f1() { if (n->Nav(t->GetPos(), e.GetPItem()->GetPos(), t->GetCM().GetP())) return true; return false; }
bool A::f2() { if (n->NavL(t->GetPos(), e.GetPItem()->GetPos(), t->GetCM().GetQ())) return true; return false; }
bool A::f3() { n->Nav(t->GetPos(), e.GetPItem()->GetPos(), t->GetCM().GetP()); return false; }
