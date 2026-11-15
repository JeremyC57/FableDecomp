struct E { long X, Y; E(){} E(const E& p):X(p.X),Y(p.Y){} };
struct D { const E Get(); long A, B2; long GetY2(); const long& RY(); };
struct B { virtual unsigned long W(long); virtual unsigned long H(long); virtual long HS(long); };
extern D* GD;
struct T { B& Bank; long g; float h6(); float h7(); float h8(); float h9(); float h10(); float h11(); float h12(); };
float T::h6() { return (float)(unsigned long)(Bank.HS(g) * GD->Get().Y); }
float T::h7() { return (float)(Bank.H(g) * GD->B2); }
float T::h8() { return (float)(Bank.H(g) * GD->GetY2()); }
float T::h9() { return (float)(Bank.H(g) * GD->RY()); }
float T::h10() { return (float)(Bank.H(g) * (GD->Get().Y + 0)); }
float T::h11() { return (float)(GD->Get().Y * Bank.H(g)); }
float T::h12() { unsigned long h = Bank.H(g); return (float)(h * GD->Get().Y); }
