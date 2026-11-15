#pragma optimize("s", on)
struct E { long X, Y; E(){} E(const E& p):X(p.X),Y(p.Y){} };
struct D { const E Get(); const E& RE(); const long* RP(); long* A; long M; };
struct B { virtual unsigned long W(unsigned long) const; virtual unsigned long H(unsigned long) const; };
extern D* GD;
struct G { long L0; long pad[500]; long L; };
struct T { const B& Bank; long g; long* P;
 float x4(const G&); float x5(const G&); float x6(const G&); float x7(const G&); float x8(const G&); float x9(const G&);};
float T::x4(const G& q) { return (float)(Bank.W(q.L) * GD->RE().X); }
float T::x5(const G& q) { return (float)(Bank.W(q.L) * *GD->RP()); }
float T::x6(const G& q) { return (float)(Bank.W(q.L) * GD->RP()[1]); }
float T::x7(const G& q) { return (float)(Bank.W(q.L) * *GD->A); }
float T::x8(const G& q) { return (float)(Bank.W(q.L) * GD->A[1]); }
float T::x9(const G& q) { return (float)(Bank.W(q.L) * P[g]); }
