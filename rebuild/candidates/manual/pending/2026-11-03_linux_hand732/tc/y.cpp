struct V3 { float x,y,z; };
struct M { int pad[10]; float P; const float GC() const { return P; } float GO() const; };
struct C { M& GetM(); const V3& GetPos() const; };
struct N { bool Able(const V3&, const V3&, float) const; };
int f1(C* me, C* t, N* n){ if (n->Able(me->GetPos(), t->GetPos(), me->GetM().GC())) return 1; return 2; }
float M::GO() const { return P; }
const float (M::*pp)() const = &M::GC;
