struct S; typedef int (__fastcall* VFn)(S*);
struct S { VFn* vtbl; };
bool __fastcall IsOuterRingSequence(S* sequence){ return sequence->vtbl[0x3c/4](sequence) == 0x80; }