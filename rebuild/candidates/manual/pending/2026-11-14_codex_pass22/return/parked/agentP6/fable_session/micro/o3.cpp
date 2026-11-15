struct E { long X, Y; E(){} E(const E& p):X(p.X),Y(p.Y){} long GetY() const { return Y; } const long& RefY() const { return Y; } };
struct D { const E Get(); const E& RE(); long GetHeight() { return Get().Y; } };
struct B { virtual unsigned long W(long); virtual unsigned long H(long); virtual long HS(long); };
extern D* GD;
static inline const long& YOf(const E& e) { return e.Y; }
static inline long YOfV(const E& e) { return e.Y; }
struct T { B& Bank; long g; float h13(); float h14(); float h15(); float h16(); float h17(); float h18(); float h19(); };
float T::h13() { return (float)(Bank.H(g) * GD->RE().Y); }
float T::h14() { return (float)(Bank.H(g) * YOf(GD->Get())); }
float T::h15() { return (float)(Bank.H(g) * YOfV(GD->Get())); }
float T::h16() { return (float)(Bank.H(g) * GD->GetHeight()); }
float T::h17() { return (float)(Bank.H(g) * GD->Get().GetY()); }
float T::h18() { return (float)(Bank.H(g) * E(GD->Get()).Y); }
float T::h19() { return (float)(Bank.H(g) * GD->Get().RefY()); }
