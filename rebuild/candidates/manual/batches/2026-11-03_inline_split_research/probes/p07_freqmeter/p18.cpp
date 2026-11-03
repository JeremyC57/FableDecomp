// p18: do { } while (0) blocks in the static frequency estimate
struct Info { unsigned long RefCount; void (__fastcall* DeleteFunc)(void*); void* Data; };
void operator delete(void*);
struct Handle { void* Data; Info* I; };
inline void Release(Handle& h) { if (h.I) { if (--h.I->RefCount == 0) { h.I->DeleteFunc(h.I->Data); operator delete(h.I); } } h.Data = 0; h.I = 0; }
bool Test(int); bool Term(); void G(int); extern Handle H1;

void a_dw0() { do { Release(H1); } while (0); G(0); }
void b_dw0_if() { do { if (Test(1)) Release(H1); } while (0); G(0); }
void c_if_dw0() { if (Test(1)) do { Release(H1); } while (0); G(0); }
void d_after_dw0() { do { G(1); } while (0); Release(H1); G(0); }
void e_dw0_twice() { do { G(1); } while (0); do { Release(H1); } while (0); G(0); }
void f_dw0_in_cond() { do { if (!Test(1)) { Release(H1); } } while (0); G(0); }
void g_loop_dw0ret_r0() { for (;;) { do { if (Term()) { Release(H1); return; } } while (0); G(0); do { if (Term()) return; } while (0); G(1); do { if (Term()) return; } while (0); G(2); do { if (Term()) return; } while (0); G(3); } }
void g_loop_dw0ret_r1() { for (;;) { do { if (Term()) return; } while (0); G(0); do { if (Term()) { Release(H1); return; } } while (0); G(1); do { if (Term()) return; } while (0); G(2); do { if (Term()) return; } while (0); G(3); } }
void g_loop_dw0ret_r2() { for (;;) { do { if (Term()) return; } while (0); G(0); do { if (Term()) return; } while (0); G(1); do { if (Term()) { Release(H1); return; } } while (0); G(2); do { if (Term()) return; } while (0); G(3); } }
void g_loop_dw0ret_r3() { for (;;) { do { if (Term()) return; } while (0); G(0); do { if (Term()) return; } while (0); G(1); do { if (Term()) return; } while (0); G(2); do { if (Term()) { Release(H1); return; } } while (0); G(3); } }
void h_loop_mixed_r0() { for (;;) { do { if (Term()) { Release(H1); return; } } while (0); G(0); do { if (Term()) return; } while (0); G(1); if (Term()) return; G(2); if (Term()) return; G(3); } }
void h_loop_mixed_r1() { for (;;) { do { if (Term()) return; } while (0); G(0); do { if (Term()) { Release(H1); return; } } while (0); G(1); if (Term()) return; G(2); if (Term()) return; G(3); } }
void h_loop_mixed_r2() { for (;;) { do { if (Term()) return; } while (0); G(0); do { if (Term()) return; } while (0); G(1); if (Term()) { Release(H1); return; } G(2); if (Term()) return; G(3); } }
void h_loop_mixed_r3() { for (;;) { do { if (Term()) return; } while (0); G(0); do { if (Term()) return; } while (0); G(1); if (Term()) return; G(2); if (Term()) { Release(H1); return; } G(3); } }
