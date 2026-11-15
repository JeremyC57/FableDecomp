#pragma optimize("s", on)
struct E { long X, Y; E(){} E(const E& p):X(p.X),Y(p.Y){} const E& Me() const { return *this; } };
struct D { const E Get(); long M; const E& GetR() { return Get(); } };
struct B { virtual unsigned long W(unsigned long) const; virtual unsigned long H(unsigned long) const; };
extern D* GD;
struct G { long L0; long pad[500]; long L; };
inline const E& Self(const E& e) { return e; }
template <class T> inline const T& Ref(const T& e) { return e; }
struct T { const B& Bank; long g;
 float e1(const G&); float e2(const G&); float e3(const G&); float e4(const G&); float e5(const G&); float e6(const G&);};
float T::e1(const G& q) { return (float)(Bank.H(q.L) * Self(GD->Get()).Y); }
float T::e2(const G& q) { return (float)(Bank.H(q.L) * GD->Get().Me().Y); }
float T::e3(const G& q) { return (float)(Bank.H(q.L) * Ref(GD->Get().Y)); }
float T::e4(const G& q) { return (float)(Bank.W(q.L) * Ref(GD->Get().X)); }
float T::e5(const G& q) { return (float)(Bank.H(q.L) * (long)(GD->Get().Y)); }
float T::e6(const G& q) { const E& e = GD->Get(); unsigned long h = Bank.H(q.L); return (float)(h * e.Y); }
