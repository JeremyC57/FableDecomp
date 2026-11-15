struct E { long X, Y; E(){} E(const E& p):X(p.X),Y(p.Y){} };
struct D { const E Get(); const E Get1(long); long M; };
struct B { virtual unsigned long W(unsigned long) const; virtual unsigned long H(unsigned long) const; virtual unsigned long H2(long,long) const; virtual unsigned long H3(long,long,long) const;};
extern D* GD; extern long GL; extern D** GDD;
struct G { long L0; long pad[500]; long L; };
struct T { const B& Bank; long g; float c1(const G&); float c2(const G&); float c3(const G&); float c4(const G&); float c5(const G&); float c6(const G&); float c7(const G&);};
float T::c1(const G& q) { return (float)(Bank.H2(q.L,q.L0) * GD->Get().X); }
float T::c2(const G& q) { return (float)(Bank.H3(q.L,q.L0,g) * GD->Get().X); }
float T::c3(const G& q) { return (float)(Bank.H(q.L) * GD->Get1(q.L0).Y); }
float T::c4(const G& q) { return (float)(Bank.H(q.L) * (*GDD)->Get().Y); }
float T::c5(const G& q) { return (float)(Bank.H(q.L+1) * GD->Get().X); }
float T::c6(const G& q) { return (float)(Bank.H(q.L) * (GD->Get().Y + GD->M)); }
float T::c7(const G& q) { return (float)(Bank.H(q.L) * GD->Get().Y * 2); }
