// p13: block frequency of the loop body and of the return blocks as more in-loop returns are added (Release = fixed-cost marker)
struct Info { unsigned long RefCount; void (__fastcall* DeleteFunc)(void*); void* Data; };
void operator delete(void*);
struct Handle { void* Data; Info* I; };
inline void Release(Handle& h) { if (h.I) { if (--h.I->RefCount == 0) { h.I->DeleteFunc(h.I->Data); operator delete(h.I); } } h.Data = 0; h.I = 0; }
bool Test(int); bool Term(); void G(int); extern Handle H1;
void b1() { for (;;) { if (Term()) return; Release(H1); G(0); } }
void b2() { for (;;) { if (Term()) return; Release(H1); if (Test(1)) { if (Term()) return; G(1); } G(0); } }
void b3() { for (;;) { if (Term()) return; Release(H1); if (Test(1)) { if (Term()) return; if (Test(2)) { if (Term()) return; G(2); } } G(0); } }
void b4() { for (;;) { if (Term()) return; Release(H1); if (Test(1)) { if (Term()) return; if (Test(2)) { if (Term()) return; if (Test(3)) { if (Term()) return; G(3); } } } G(0); } }
void r1() { for (;;) { if (Term()) { Release(H1); return; } G(0); } }
void r2() { for (;;) { if (Term()) { Release(H1); return; } if (Test(1)) { if (Term()) return; G(1); } G(0); } }
void r3() { for (;;) { if (Term()) { Release(H1); return; } if (Test(1)) { if (Term()) return; if (Test(2)) { if (Term()) return; G(2); } } G(0); } }
void q2() { for (;;) { if (Term()) return; if (Test(1)) { if (Term()) { Release(H1); return; } G(1); } G(0); } }
void q3() { for (;;) { if (Term()) return; if (Test(1)) { if (Term()) { Release(H1); return; } if (Test(2)) { if (Term()) return; G(2); } } G(0); } }
void z3() { for (;;) { if (Term()) return; if (Test(1)) { if (Term()) return; if (Test(2)) { if (Term()) { Release(H1); return; } G(2); } } G(0); } }
