struct S; typedef int (__fastcall* VFn)(S*);
struct S { VFn* vtbl; };
bool __fastcall IsInterruptable(S* physics){ return physics->vtbl[0x174/4](physics) != 0; }