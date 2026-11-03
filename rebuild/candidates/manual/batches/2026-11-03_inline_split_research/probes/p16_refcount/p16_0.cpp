// p16: does the number of references to the callee elsewhere in the TU change a site's decision?
struct Info { unsigned long RefCount; void (__fastcall* DeleteFunc)(void*); void* Data; };
void operator delete(void*);
struct Handle { void* Data; Info* I; };
inline void Release(Handle& h) { if (h.I) { if (--h.I->RefCount == 0) { h.I->DeleteFunc(h.I->Data); operator delete(h.I); } } h.Data = 0; h.I = 0; }
static void SRelease(Handle& h) { if (h.I) { if (--h.I->RefCount == 0) { h.I->DeleteFunc(h.I->Data); operator delete(h.I); } } h.Data = 0; h.I = 0; }
bool Test(int); bool Term(); void G(int); extern Handle H1, H2;

void A_d3() { if (Test(1)) if (Test(2)) if (Test(3)) Release(H1); G(0); }
void A_d2() { if (Test(1)) if (Test(2)) Release(H1); G(0); }
void S_d3() { if (Test(1)) if (Test(2)) if (Test(3)) SRelease(H1); G(0); }
void S_d4() { if (Test(1)) if (Test(2)) if (Test(3)) if (Test(4)) SRelease(H1); G(0); }
