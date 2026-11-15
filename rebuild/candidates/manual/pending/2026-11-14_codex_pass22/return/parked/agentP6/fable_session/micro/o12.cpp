#pragma optimize("s", on)
struct E { long X, Y; E(){} E(const E& p):X(p.X),Y(p.Y){} };
struct EA { long V[2]; EA(){} };
struct EB0 { long X; }; struct EB : EB0 { long Y; EB(){} };
struct E9 { long X, Y; E9(){} };
struct D { const E Get(); const EA GetA(); const EB GetB(); E Get2(); const E9 Get9(); long M; };
struct B { virtual unsigned long W(unsigned long) const; virtual unsigned long H(unsigned long) const; };
extern D* GD;
struct G { long L0; long pad[500]; long L; };
struct T { const B& Bank; long g;
 float r1(const G&); float r2(const G&); float r8(const G&); float r9(const G&);};
float T::r1(const G& q) { return (float)(Bank.H(q.L) * GD->GetA().V[1]); }
float T::r2(const G& q) { return (float)(Bank.H(q.L) * GD->GetB().Y); }
float T::r8(const G& q) { return (float)(Bank.H(q.L) * GD->Get2().Y); }
float T::r9(const G& q) { return (float)(Bank.H(q.L) * GD->Get9().Y); }
