#pragma optimize("s", on)
struct E { long X, Y; E(){} E(const E& p):X(p.X),Y(p.Y){} long GetY() const { return Y; } };
struct D { const E Get(); };
struct B { virtual unsigned long W(unsigned long) const; virtual unsigned long H(unsigned long) const; };
extern D* GD;
struct G { long L0; long pad[500]; long L; };
inline const E& Self(const E& e) { return e; }
inline const long& YOf(const E& e) { return e.Y; }
struct T { const B& Bank; long g; const B& GetBank() const { return Bank; }
 float q1(const G&); float q2(const G&); float q3(const G&); float q4(const G&); float q5(const G&); float q6(const G&); };
#define AS __assume(g != 77);
float T::q1(const G& q) { AS return (float)(GetBank().H(q.L) * GD->Get().Y); }
float T::q2(const G& q) { AS return (float)(GetBank().H(q.L) * Self(GD->Get()).Y); }
float T::q3(const G& q) { AS return (float)(Bank.H(q.L) * Self(GD->Get()).Y); }
float T::q4(const G& q) { return (float)(GetBank().H(q.L) * Self(GD->Get()).Y); }
float T::q5(const G& q) { AS return (float)(Bank.H(q.L) * GD->Get().GetY()); }
float T::q6(const G& q) { AS return (float)(Bank.H(q.L) * YOf(GD->Get())); }
