// p15: per-exit frequency for goto / while / do-while / continue loops (marker in one exit block per function)
struct Info { unsigned long RefCount; void (__fastcall* DeleteFunc)(void*); void* Data; };
void operator delete(void*);
struct Handle { void* Data; Info* I; };
inline void Release(Handle& h) { if (h.I) { if (--h.I->RefCount == 0) { h.I->DeleteFunc(h.I->Data); operator delete(h.I); } } h.Data = 0; h.I = 0; }
bool Test(int); bool Term(); void G(int); extern Handle H1;
void gotoloop_r0() { top: if (Term()) { Release(H1); return; } if (Test(1)) { if (Term()) return; if (Test(2)) { if (Term()) return; if (Test(3)) { if (Term()) return; G(3); } } } G(0); goto top; }
void whileloop_r0() { while (!Test(9)) { if (Term()) { Release(H1); return; } if (Test(1)) { if (Term()) return; if (Test(2)) { if (Term()) return; if (Test(3)) { if (Term()) return; G(3); } } } G(0); } }
void dowhile_r0() { do { if (Term()) { Release(H1); return; } if (Test(1)) { if (Term()) return; if (Test(2)) { if (Term()) return; if (Test(3)) { if (Term()) return; G(3); } } } G(0); } while (!Test(9)); }
void contloop_r0() { for (;;) { if (Term()) { Release(H1); return; } if (Test(1)) { if (Term()) return; if (Test(2)) { if (Term()) return; if (Test(3)) { if (Term()) return; G(3); continue; } G(2); continue; } } G(0); } }
void gotoloop_r1() { top: if (Term()) return; if (Test(1)) { if (Term()) { Release(H1); return; } if (Test(2)) { if (Term()) return; if (Test(3)) { if (Term()) return; G(3); } } } G(0); goto top; }
void whileloop_r1() { while (!Test(9)) { if (Term()) return; if (Test(1)) { if (Term()) { Release(H1); return; } if (Test(2)) { if (Term()) return; if (Test(3)) { if (Term()) return; G(3); } } } G(0); } }
void dowhile_r1() { do { if (Term()) return; if (Test(1)) { if (Term()) { Release(H1); return; } if (Test(2)) { if (Term()) return; if (Test(3)) { if (Term()) return; G(3); } } } G(0); } while (!Test(9)); }
void contloop_r1() { for (;;) { if (Term()) return; if (Test(1)) { if (Term()) { Release(H1); return; } if (Test(2)) { if (Term()) return; if (Test(3)) { if (Term()) return; G(3); continue; } G(2); continue; } } G(0); } }
void gotoloop_r2() { top: if (Term()) return; if (Test(1)) { if (Term()) return; if (Test(2)) { if (Term()) { Release(H1); return; } if (Test(3)) { if (Term()) return; G(3); } } } G(0); goto top; }
void whileloop_r2() { while (!Test(9)) { if (Term()) return; if (Test(1)) { if (Term()) return; if (Test(2)) { if (Term()) { Release(H1); return; } if (Test(3)) { if (Term()) return; G(3); } } } G(0); } }
void dowhile_r2() { do { if (Term()) return; if (Test(1)) { if (Term()) return; if (Test(2)) { if (Term()) { Release(H1); return; } if (Test(3)) { if (Term()) return; G(3); } } } G(0); } while (!Test(9)); }
void contloop_r2() { for (;;) { if (Term()) return; if (Test(1)) { if (Term()) return; if (Test(2)) { if (Term()) { Release(H1); return; } if (Test(3)) { if (Term()) return; G(3); continue; } G(2); continue; } } G(0); } }
void gotoloop_r3() { top: if (Term()) return; if (Test(1)) { if (Term()) return; if (Test(2)) { if (Term()) return; if (Test(3)) { if (Term()) { Release(H1); return; } G(3); } } } G(0); goto top; }
void whileloop_r3() { while (!Test(9)) { if (Term()) return; if (Test(1)) { if (Term()) return; if (Test(2)) { if (Term()) return; if (Test(3)) { if (Term()) { Release(H1); return; } G(3); } } } G(0); } }
void dowhile_r3() { do { if (Term()) return; if (Test(1)) { if (Term()) return; if (Test(2)) { if (Term()) return; if (Test(3)) { if (Term()) { Release(H1); return; } G(3); } } } G(0); } while (!Test(9)); }
void contloop_r3() { for (;;) { if (Term()) return; if (Test(1)) { if (Term()) return; if (Test(2)) { if (Term()) return; if (Test(3)) { if (Term()) { Release(H1); return; } G(3); continue; } G(2); continue; } } G(0); } }

