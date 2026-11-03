// p14: per-exit frequency in loops with many exits (marker in one exit block per function)
struct Info { unsigned long RefCount; void (__fastcall* DeleteFunc)(void*); void* Data; };
void operator delete(void*);
struct Handle { void* Data; Info* I; };
inline void Release(Handle& h) { if (h.I) { if (--h.I->RefCount == 0) { h.I->DeleteFunc(h.I->Data); operator delete(h.I); } } h.Data = 0; h.I = 0; }
bool Test(int); bool Term(); void G(int); extern Handle H1;

void nest5_r0() { for (;;) { if (Term()) { Release(H1); return; } if (Test(1)) { if (Term()) return; if (Test(2)) { if (Term()) return; if (Test(3)) { if (Term()) return; if (Test(4)) { if (Term()) return; G(4); } } } } G(0); } }
void nest5_r1() { for (;;) { if (Term()) return; if (Test(1)) { if (Term()) { Release(H1); return; } if (Test(2)) { if (Term()) return; if (Test(3)) { if (Term()) return; if (Test(4)) { if (Term()) return; G(4); } } } } G(0); } }
void nest5_r2() { for (;;) { if (Term()) return; if (Test(1)) { if (Term()) return; if (Test(2)) { if (Term()) { Release(H1); return; } if (Test(3)) { if (Term()) return; if (Test(4)) { if (Term()) return; G(4); } } } } G(0); } }
void nest5_r3() { for (;;) { if (Term()) return; if (Test(1)) { if (Term()) return; if (Test(2)) { if (Term()) return; if (Test(3)) { if (Term()) { Release(H1); return; } if (Test(4)) { if (Term()) return; G(4); } } } } G(0); } }
void nest5_r4() { for (;;) { if (Term()) return; if (Test(1)) { if (Term()) return; if (Test(2)) { if (Term()) return; if (Test(3)) { if (Term()) return; if (Test(4)) { if (Term()) { Release(H1); return; } G(4); } } } } G(0); } }
void flat5_r0() { for (;;) { if (Term()) { Release(H1); return; } G(0); if (Term()) return; G(1); if (Term()) return; G(2); if (Term()) return; G(3); if (Term()) return; G(4); } }
void flat5_r1() { for (;;) { if (Term()) return; G(0); if (Term()) { Release(H1); return; } G(1); if (Term()) return; G(2); if (Term()) return; G(3); if (Term()) return; G(4); } }
void flat5_r2() { for (;;) { if (Term()) return; G(0); if (Term()) return; G(1); if (Term()) { Release(H1); return; } G(2); if (Term()) return; G(3); if (Term()) return; G(4); } }
void flat5_r3() { for (;;) { if (Term()) return; G(0); if (Term()) return; G(1); if (Term()) return; G(2); if (Term()) { Release(H1); return; } G(3); if (Term()) return; G(4); } }
void flat5_r4() { for (;;) { if (Term()) return; G(0); if (Term()) return; G(1); if (Term()) return; G(2); if (Term()) return; G(3); if (Term()) { Release(H1); return; } G(4); } }
void line5_r0() { if (Term()) { Release(H1); return; } G(0); if (Term()) return; G(1); if (Term()) return; G(2); if (Term()) return; G(3); if (Term()) return; G(4); }
void line5_r1() { if (Term()) return; G(0); if (Term()) { Release(H1); return; } G(1); if (Term()) return; G(2); if (Term()) return; G(3); if (Term()) return; G(4); }
void line5_r2() { if (Term()) return; G(0); if (Term()) return; G(1); if (Term()) { Release(H1); return; } G(2); if (Term()) return; G(3); if (Term()) return; G(4); }
void line5_r3() { if (Term()) return; G(0); if (Term()) return; G(1); if (Term()) return; G(2); if (Term()) { Release(H1); return; } G(3); if (Term()) return; G(4); }
void line5_r4() { if (Term()) return; G(0); if (Term()) return; G(1); if (Term()) return; G(2); if (Term()) return; G(3); if (Term()) { Release(H1); return; } G(4); }
