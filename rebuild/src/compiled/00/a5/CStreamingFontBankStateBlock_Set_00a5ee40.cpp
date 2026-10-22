struct P { char a[4]; int f4,f8,fc; unsigned char f10; };
struct C { char a[4]; int f4,f8,fc; unsigned char f10; void Set(P& block); };
void C::Set(P& block){ this->f4=block.f4; this->f8=block.f8; this->fc=block.fc; this->f10=block.f10; }