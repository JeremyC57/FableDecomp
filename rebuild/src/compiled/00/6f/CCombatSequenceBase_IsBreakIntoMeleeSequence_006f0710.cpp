// Leaf: ?CCombatSequenceBase_IsBreakIntoMeleeSequence@@YI_NPAUS@@@Z
struct S; typedef int (__fastcall* VFn)(S*);
struct S { VFn* vtbl; };
bool __fastcall CCombatSequenceBase_IsBreakIntoMeleeSequence(S* sequence)
{
    return sequence->vtbl[0xc/4](sequence) == 2;
}