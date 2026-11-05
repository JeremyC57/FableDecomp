// Leaf: ?CCombatSequenceBase_IsBoastSequence@@YI_NPAUS@@@Z
struct S; typedef int (__fastcall* VFn)(S*);
struct S { VFn* vtbl; };
bool __fastcall CCombatSequenceBase_IsBoastSequence(S* sequence)
{
    return sequence->vtbl[0xc/4](sequence) == 3;
}