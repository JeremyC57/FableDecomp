// p08: sequential loops -- does the block after a loop inherit the loop's frequency ("leak")?
struct Info { unsigned long RefCount; void (__fastcall* DeleteFunc)(void*); void* Data; };
void operator delete(void*);
struct Handle { void* Data; Info* I; };
inline void Release(Handle& h) { if (h.I) { if (--h.I->RefCount == 0) { h.I->DeleteFunc(h.I->Data); operator delete(h.I); } } h.Data = 0; h.I = 0; }
bool Test(int); bool Term(); void G(int); extern Handle H1, H2; extern int N; extern int* B; extern int* E; extern int* B2; extern int* E2;
void a_after_forN() { for (int i = 0; i < N; ++i) G(i); Release(H1); G(0); }
void b_after_ptr() { for (int* p = B; p != E; ++p) G(*p); Release(H1); G(0); }
void c_after_while() { while (Test(1)) G(1); Release(H1); G(0); }
void d_after_forN_ifexit() { for (int i = 0; i < N; ++i) { if (Test(i)) break; G(i); } Release(H1); G(0); }
void e_two_ptr_loops() { for (int* p = B; p != E; ++p) { Release(H1); G(*p); } for (int* q = B2; q != E2; ++q) { Release(H2); G(*q); } }
void f_two_forN_loops() { for (int i = 0; i < N; ++i) { Release(H1); G(i); } for (int j = 0; j < N; ++j) { Release(H2); G(j); } }
void g_after_two_loops() { for (int i = 0; i < N; ++i) G(i); for (int j = 0; j < N; ++j) G(j); Release(H1); G(0); }
void h_after_forN_member() { for (int i = 0; i < N; ++i) G(i); if (Test(1)) Release(H1); G(0); }
