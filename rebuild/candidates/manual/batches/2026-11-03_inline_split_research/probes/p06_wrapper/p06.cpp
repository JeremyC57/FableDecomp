// p06: Hypothesis 1 -- does an inline wrapper layer change the inner callee's decision?
struct Info { unsigned long RefCount; void (__fastcall* DeleteFunc)(void*); void* Data; };
void operator delete(void*);
struct Handle { void* Data; Info* I; };
inline void Release(Handle& h) { if (h.I) { if (--h.I->RefCount == 0) { h.I->DeleteFunc(h.I->Data); operator delete(h.I); } } h.Data = 0; h.I = 0; }
inline void W1(Handle& h) { Release(h); }
inline void W2(Handle& h) { W1(h); }
inline void W3(Handle& h) { W2(h); }
inline void W4(Handle& h) { W3(h); }
inline void W5(Handle& h) { W4(h); }
inline void W6(Handle& h) { W5(h); }
inline void W7(Handle& h) { W6(h); }
inline void W8(Handle& h) { W7(h); }
inline void W9(Handle& h) { W8(h); }
inline void W10(Handle& h) { W9(h); }
bool Test(int); void G(int); extern Handle H1;
void w0_d0() { Release(H1); G(0); }
void w1_d0() { W1(H1); G(0); }
void w2_d0() { W2(H1); G(0); }
void w4_d0() { W4(H1); G(0); }
void w6_d0() { W6(H1); G(0); }
void w7_d0() { W7(H1); G(0); }
void w8_d0() { W8(H1); G(0); }
void w9_d0() { W9(H1); G(0); }
void w10_d0() { W10(H1); G(0); }
void w0_d2() { if (Test(1)) if (Test(2)) Release(H1); G(0); }
void w1_d2() { if (Test(1)) if (Test(2)) W1(H1); G(0); }
void w2_d2() { if (Test(1)) if (Test(2)) W2(H1); G(0); }
void w4_d2() { if (Test(1)) if (Test(2)) W4(H1); G(0); }
