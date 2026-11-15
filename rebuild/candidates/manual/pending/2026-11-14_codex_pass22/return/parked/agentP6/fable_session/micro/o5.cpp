struct E { long X, Y; E(){} E(const E& p):X(p.X),Y(p.Y){} };
struct D { const E Get(); };
struct B { virtual unsigned long W(long); virtual unsigned long H(long); unsigned long NH(long); virtual unsigned long H0(); virtual unsigned long H2(long,long);};
extern D* GD;
struct G { long pad[500]; long L; };
struct T { B& Bank; long g; B* PB; G& gd;
 float a1(); float a2(); float a3(); float a4(); float a5(const G&); float a6(); float a7(const G&); float a8(const G&);};
float T::a1() { return (float)(Bank.H(5) * GD->Get().Y); }
float T::a2() { return (float)(Bank.NH(g) * GD->Get().Y); }
float T::a3() { return (float)(Bank.H0() * GD->Get().Y); }
float T::a4() { return (float)(PB->H(g) * GD->Get().Y); }
float T::a5(const G& q) { return (float)(Bank.H(q.L) * GD->Get().Y); }
float T::a6() { return (float)(Bank.H2(g,g) * GD->Get().Y); }
float T::a7(const G& q) { float w=(float)(Bank.W(q.L) * GD->Get().X)/640.0f; float h=(float)(Bank.H(q.L) * GD->Get().Y)/480.0f; return w+h; }
float T::a8(const G& q) { return (float)(Bank.H(q.L) * GD->Get().Y) / 480.0f; }
