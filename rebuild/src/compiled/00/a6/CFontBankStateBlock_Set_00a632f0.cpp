struct P { char a[4]; int f4,f8; unsigned char fc; };
struct C { char a[4]; int f4,f8; unsigned char fc; void Set(P& block); };
void C::Set(P& block){ this->f4=block.f4; this->f8=block.f8; this->fc=block.fc; }