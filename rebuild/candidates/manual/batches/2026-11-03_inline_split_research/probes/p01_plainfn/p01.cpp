// p01: plain inline function (not a dtor) called at 5 straight-line sites and at 5 return exits
struct Info { unsigned long RefCount; void (__fastcall* DeleteFunc)(void*); void* Data; };
void operator delete(void*);
struct Handle { void* Data; Info* I; };
inline void Rel(Handle& h) { if (h.I) { if (--h.I->RefCount == 0) { h.I->DeleteFunc(h.I->Data); operator delete(h.I); } } h.Data = 0; h.I = 0; }
bool Test(int); void Yield(); extern Handle G1, G2, G3, G4, G5;
void s5() { Rel(G1); Yield(); Rel(G2); Yield(); Rel(G3); Yield(); Rel(G4); Yield(); Rel(G5); }
void e5() { if (Test(1)) { Rel(G1); return; } Yield(); if (Test(2)) { Rel(G1); return; } Yield(); if (Test(3)) { Rel(G1); return; } Yield(); if (Test(4)) { Rel(G1); return; } Yield(); Rel(G1); }
