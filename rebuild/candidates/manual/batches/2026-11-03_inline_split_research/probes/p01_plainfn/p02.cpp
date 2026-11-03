// p02: plain inline function, one site per function at nesting depth d of if-conditions (static prob 1/2 each)
struct Info { unsigned long RefCount; void (__fastcall* DeleteFunc)(void*); void* Data; };
void operator delete(void*);
struct Handle { void* Data; Info* I; };
inline void Rel(Handle& h) { if (h.I) { if (--h.I->RefCount == 0) { h.I->DeleteFunc(h.I->Data); operator delete(h.I); } } h.Data = 0; h.I = 0; }
bool Test(int); void Yield(); extern Handle G1;
void d0() { Rel(G1); Yield(); }
void d1() { if (Test(1)) Rel(G1); Yield(); }
void d2() { if (Test(1)) if (Test(2)) Rel(G1); Yield(); }
void d3() { if (Test(1)) if (Test(2)) if (Test(3)) Rel(G1); Yield(); }
void d4() { if (Test(1)) if (Test(2)) if (Test(3)) if (Test(4)) Rel(G1); Yield(); }
void d5() { if (Test(1)) if (Test(2)) if (Test(3)) if (Test(4)) if (Test(5)) Rel(G1); Yield(); }
void l1() { for (int i = 0; i < 10; ++i) { if (Test(1)) if (Test(2)) if (Test(3)) if (Test(4)) if (Test(5)) Rel(G1); } Yield(); }
