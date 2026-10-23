struct P { char a[0x80]; int f80; char b[0x8c-0x84]; int f8c; };
struct Sub { void MX(void* p); void MY(void* p); };
struct C { char a[0x150]; Sub sub; void BaseUpdate_00bd2bc0(P& p, unsigned long k); void U(P& p, unsigned long k); };
void C::U(P& p, unsigned long k){ this->BaseUpdate_00bd2bc0(p,k); this->sub.MX(&p.f80); this->sub.MY(&p.f8c); }