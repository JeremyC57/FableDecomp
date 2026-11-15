struct E { long X, Y; E(){} E(const E& p):X(p.X),Y(p.Y){} };
struct D { const E Get(); };
struct B { virtual unsigned long W(unsigned long) const; virtual unsigned long H(unsigned long) const; };
extern D* GD; extern long GL;
struct G { long L0; long pad[500]; long L; };
struct T { const B& Bank; float b1(long v); float b2(const G&); float b3(); float b4(const G&); float b5(const G*); float b6(const G&);};
float T::b1(long v) { return (float)(Bank.H(v) * GD->Get().Y); }
float T::b2(const G& q) { return (float)(Bank.H(q.L0) * GD->Get().Y); }
float T::b3() { return (float)(Bank.H(GL) * GD->Get().Y); }
float T::b4(const G& q) { return (float)(Bank.H(q.L) * GD->Get().Y); }
float T::b5(const G* q) { return (float)(Bank.H(q->L) * GD->Get().Y); }
float T::b6(const G& q) { return (float)(Bank.H(q.L) * GD->Get().X); }
