struct E { long X, Y; E(){} E(const E& p):X(p.X),Y(p.Y){} };
struct D { const E Get(); const E Wrap() { return Get(); } };
struct B { virtual unsigned long W(long); virtual unsigned long H(long); virtual long HS(long); };
extern D* GD;
inline const E GWrap() { return GD->Get(); }
inline D* GetD() { return GD; }
struct T { B& Bank; long g; float h20(); float h21(); float h22(); float h23(); float w20();};
float T::h20() { return (float)(Bank.H(g) * GD->Wrap().Y); }
float T::w20() { return (float)(Bank.W(g) * GD->Wrap().X); }
float T::h21() { return (float)(Bank.H(g) * GWrap().Y); }
float T::h22() { return (float)(Bank.H(g) * GetD()->Get().Y); }
float T::h23() { return (float)(Bank.H(g) * (const unsigned long&)GD->Get().Y); }
