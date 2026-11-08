// constant-value probe: a non-zero constant stored 6 times with a call after each store becomes a callee-saved
// register candidate (research861 consts.cpp: 5+ stores with calls -> register). Values 1234 and -77.
struct S { int f[16]; }; void call();
void k1(S* p) { p->f[0] = 1234; call(); p->f[1] = 1234; call(); p->f[2] = 1234; call(); p->f[3] = 1234; call(); p->f[4] = 1234; call(); p->f[5] = 1234; call(); }
void k2(S* p) { p->f[0] = -77; call(); p->f[1] = -77; call(); p->f[2] = -77; call(); p->f[3] = -77; call(); p->f[4] = -77; call(); p->f[5] = -77; call(); }
void k3(S* p) { p->f[0] = 0x12345678; call(); p->f[1] = 0x12345678; call(); p->f[2] = 0x12345678; call(); p->f[3] = 0x12345678; call(); p->f[4] = 0x12345678; call(); p->f[5] = 0x12345678; call(); }
