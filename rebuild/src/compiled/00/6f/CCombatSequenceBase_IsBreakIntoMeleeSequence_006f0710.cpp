struct S; typedef int (__fastcall* VFn)(S*);
struct S { VFn* vtbl; };
bool __fastcall IsBreakIntoMeleeSequence(S* sequence)
{
    return sequence->vtbl[0xc/4](sequence) == 2;
}