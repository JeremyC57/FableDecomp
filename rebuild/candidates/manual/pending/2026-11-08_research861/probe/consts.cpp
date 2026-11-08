struct S { int f[16]; }; void call();
void z1(S* p) { p->f[0] = 0; }
void zc1(S* p) { p->f[0] = 0; call(); }
void z2(S* p) { p->f[0] = 0; p->f[1] = 0; }
void zc2(S* p) { p->f[0] = 0; call(); p->f[1] = 0; call(); }
void z3(S* p) { p->f[0] = 0; p->f[1] = 0; p->f[2] = 0; }
void zc3(S* p) { p->f[0] = 0; call(); p->f[1] = 0; call(); p->f[2] = 0; call(); }
void z4(S* p) { p->f[0] = 0; p->f[1] = 0; p->f[2] = 0; p->f[3] = 0; }
void zc4(S* p) { p->f[0] = 0; call(); p->f[1] = 0; call(); p->f[2] = 0; call(); p->f[3] = 0; call(); }
void z5(S* p) { p->f[0] = 0; p->f[1] = 0; p->f[2] = 0; p->f[3] = 0; p->f[4] = 0; }
void zc5(S* p) { p->f[0] = 0; call(); p->f[1] = 0; call(); p->f[2] = 0; call(); p->f[3] = 0; call(); p->f[4] = 0; call(); }
void z6(S* p) { p->f[0] = 0; p->f[1] = 0; p->f[2] = 0; p->f[3] = 0; p->f[4] = 0; p->f[5] = 0; }
void zc6(S* p) { p->f[0] = 0; call(); p->f[1] = 0; call(); p->f[2] = 0; call(); p->f[3] = 0; call(); p->f[4] = 0; call(); p->f[5] = 0; call(); }
void z7(S* p) { p->f[0] = 0; p->f[1] = 0; p->f[2] = 0; p->f[3] = 0; p->f[4] = 0; p->f[5] = 0; p->f[6] = 0; }
void zc7(S* p) { p->f[0] = 0; call(); p->f[1] = 0; call(); p->f[2] = 0; call(); p->f[3] = 0; call(); p->f[4] = 0; call(); p->f[5] = 0; call(); p->f[6] = 0; call(); }
