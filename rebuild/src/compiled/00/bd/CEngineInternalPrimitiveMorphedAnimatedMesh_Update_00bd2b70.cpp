struct P { char a[0x90]; int f90; char b[0x9c-0x94]; int f9c; char c[0xa8-0xa0]; int fa8; };
struct Sub { void MX_00becde0(void* p); void MY_00becba0(void* p); void MZ(void* p); };
struct C { char a[0x128]; Sub sub; void BaseUpdate_00bd2920(P& p, unsigned long k); void U(P& p, unsigned long k); };
void C::U(P& p, unsigned long k){ this->BaseUpdate_00bd2920(p,k); this->sub.MX_00becde0(&p.f9c); this->sub.MY_00becba0(&p.f90); this->sub.MZ(&p.fa8); }