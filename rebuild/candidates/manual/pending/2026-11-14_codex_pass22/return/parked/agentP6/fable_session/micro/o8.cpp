struct E { long X, Y; E(){} E(const E& p):X(p.X),Y(p.Y){} };
struct D { const E Get(); long M; };
struct B { virtual unsigned long W(unsigned long) const; virtual unsigned long H(unsigned long) const; unsigned long N0() const; };
extern D* GD; extern long GL;
struct G { long L0; long pad[500]; long L; };
extern G GG; const G& PeekG();
struct T { const B& Bank; long g; unsigned long MaxH(unsigned long x) const { return Bank.H(x); }
 float d1(const G&); float d2(); float d3(const G&); float d4(const G&); float d5(const G&); float d6(const G&); float d7(const G&); float d8(const G&);};
float T::d1(const G& q) { return (float)(MaxH(q.L) * GD->Get().Y); }
float T::d2() { return (float)(Bank.H(GG.L) * GD->Get().Y); }
float T::d3(const G& q) { return (float)(Bank.H((unsigned long)q.L) * GD->Get().Y); }
float T::d4(const G& q) { return (float)(g * GD->Get().Y); }
float T::d5(const G& q) { return (float)((unsigned long)(q.L+g) * GD->Get().Y); }
float T::d6(const G& q) { return (float)(Bank.N0() * GD->Get().Y); }
float T::d7(const G& q) { return (float)(Bank.H(q.L) * (unsigned long)GD->Get().Y); }
float T::d8(const G& q) { return (float)(Bank.H(q.L) * GD->M); }
