// p04: frequency meter on loop shapes. Release(h) is a plain-inline function; its expansions show as calls to operator delete.
struct Info { unsigned long RefCount; void (__fastcall* DeleteFunc)(void*); void* Data; };
void operator delete(void*);
struct Handle { void* Data; Info* I; };
inline void Release(Handle& h) { if (h.I) { if (--h.I->RefCount == 0) { h.I->DeleteFunc(h.I->Data); operator delete(h.I); } } h.Data = 0; h.I = 0; }
bool Test(int); bool Term(); void Yield(); extern Handle G1, G2, G3, G4;
void a_d0() { Release(G1); }
void a_d1() { if (Test(1)) Release(G1); }
void a_d2() { if (Test(1)) if (Test(2)) Release(G1); }
void a_d3() { if (Test(1)) if (Test(2)) if (Test(3)) Release(G1); }
void a_d4() { if (Test(1)) if (Test(2)) if (Test(3)) if (Test(4)) Release(G1); }
// loop body site
void b_forbody() { for (;;) { if (Term()) return; Release(G1); Yield(); } }
void b_forexit() { for (;;) { if (Term()) { Release(G1); return; } Yield(); } }
void b_whileafter() { while (!Term()) Yield(); Release(G1); }
void b_for10body() { for (int i = 0; i < 10; ++i) { Release(G1); Yield(); } }
void b_for10after() { for (int i = 0; i < 10; ++i) Yield(); Release(G1); }
void b_forbody_d2() { for (;;) { if (Term()) return; if (Test(1)) if (Test(2)) Release(G1); Yield(); } }
void b_forexit2() { for (;;) { if (Term()) { Release(G1); return; } if (Test(1)) { Release(G2); return; } Yield(); } }
void b_after_loop_with_return() { for (;;) { if (Term()) return; if (Test(1)) break; Yield(); } Release(G1); }
