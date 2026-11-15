struct E { long X, Y; E(){} E(const E& p):X(p.X),Y(p.Y){} };
struct D { const E Get(); };
struct B { virtual unsigned long W(long); virtual unsigned long H(long); };
extern D* GD;
struct T { B& Bank; long g; float w1(); float h1(); float h2(); float h3(); float h4(); float h5(); };
float T::w1() { return (float)(Bank.W(g) * GD->Get().X); }
float T::h1() { return (float)(Bank.H(g) * GD->Get().Y); }
float T::h2() { return (float)(Bank.H(g) * (unsigned long)GD->Get().Y); }
float T::h3() { const E& e = GD->Get(); return (float)(Bank.H(g) * e.Y); }
float T::h4() { return (float)(Bank.H(g) * (&GD->Get())->Y); }
float T::h5() { long E::*p = &E::Y; return (float)(Bank.H(g) * GD->Get().*p); }
