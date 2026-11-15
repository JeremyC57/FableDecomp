#pragma optimize("s", on)
struct E { long X, Y; E(){} E(const E& p):X(p.X),Y(p.Y){} };
struct D { const E Get(); long M; };
struct B { virtual unsigned long W(unsigned long) const; virtual unsigned long H(unsigned long) const; };
extern D* GD;
struct G { long L0; long pad[500]; long L; long GetL() const { return L; } const long& RefL() const { return L; } };
const G& PeekG();
struct T { const B& Bank; long g; const B& GetBank() const { return Bank; }
 float f1(const G&); float f2(const G&); float f3(const G&); float f4(const G&); float f5(); float f6(const G&);};
float T::f1(const G& q) { return (float)(Bank.H(q.GetL()) * GD->Get().Y); }
float T::f2(const G& q) { return (float)(Bank.H(q.RefL()) * GD->Get().Y); }
float T::f3(const G& q) { return (float)(GetBank().H(q.L) * GD->Get().Y); }
float T::f4(const G& q) { return (float)(GetBank().H(q.GetL()) * GD->Get().Y); }
float T::f5() { const G& q = PeekG(); float w = (float)(Bank.W(q.L) * GD->Get().X) / 640.0f; float h = (float)(Bank.H(q.L) * GD->Get().Y) / 480.0f; return w + h; }
float T::f6(const G& q) { return (float)(Bank.H(q.L) * GD->Get().Y) / 480.0f + (float)GD->Get().Y; }
