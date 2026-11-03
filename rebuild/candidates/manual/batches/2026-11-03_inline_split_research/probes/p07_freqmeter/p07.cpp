// p07: frequency meter. One Release site per function; T* (lowest /d2inlT that inlines) gives the site's score.
struct Info { unsigned long RefCount; void (__fastcall* DeleteFunc)(void*); void* Data; };
void operator delete(void*);
struct Handle { void* Data; Info* I; };
inline void Release(Handle& h) { if (h.I) { if (--h.I->RefCount == 0) { h.I->DeleteFunc(h.I->Data); operator delete(h.I); } } h.Data = 0; h.I = 0; }
bool Test(int); bool Term(); void G(int); extern Handle H1; extern int N; extern int* B; extern int* E;
void s00_entry() { Release(H1); G(0); }
void s01_if() { if (Test(1)) Release(H1); G(0); }
void s02_if2() { if (Test(1)) if (Test(2)) Release(H1); G(0); }
void s03_ifelse_else() { if (Test(1)) G(1); else Release(H1); G(0); }
void s04_andcond() { if (Test(1) && Test(2)) Release(H1); G(0); }
void s05_orcond() { if (Test(1) || Test(2)) Release(H1); G(0); }
void s06_after_if() { if (Test(1)) G(1); Release(H1); G(0); }
void s07_eqnull() { if (B == 0) Release(H1); G(0); }
void s08_neqnull() { if (B != 0) Release(H1); G(0); }
void s09_lt() { if (N < 5) Release(H1); G(0); }
void s10_eq() { if (N == 5) Release(H1); G(0); }
void s11_ne() { if (N != 5) Release(H1); G(0); }
void l00_for_n() { for (int i = 0; i < N; ++i) { Release(H1); G(i); } }
void l01_for_100() { for (int i = 0; i < 100; ++i) { Release(H1); G(i); } }
void l02_for_3() { for (int i = 0; i < 3; ++i) { Release(H1); G(i); } }
void l03_ptr() { for (int* p = B; p != E; ++p) { Release(H1); G(*p); } }
void l04_while_test() { while (Test(1)) { Release(H1); G(0); } }
void l05_forever_ret() { for (;;) { if (Term()) return; Release(H1); G(0); } }
void l06_dowhile() { do { Release(H1); G(0); } while (Test(1)); }
void l07_nested() { for (int i = 0; i < N; ++i) for (int j = 0; j < N; ++j) { Release(H1); G(j); } }
void l08_in_if_in_loop() { for (int i = 0; i < N; ++i) { if (Test(i)) Release(H1); G(i); } }
void l09_after_loop() { for (int i = 0; i < N; ++i) G(i); Release(H1); G(0); }
void l10_loop_exit_ret() { for (;;) { if (Term()) { Release(H1); return; } G(0); } }
void l11_loop_break() { for (;;) { if (Term()) break; G(0); } Release(H1); G(1); }
void l12_ptr_if() { for (int* p = B; p != E; ++p) { if (*p) Release(H1); G(*p); } }
void l13_ptr_ifelse3() { for (int* p = B; p != E; ++p) { if (Test(1)) G(1); else if (Test(2)) G(2); else if (Test(3)) Release(H1); G(*p); } }
