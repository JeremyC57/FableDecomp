// Leaf: ?CCombatSequenceBase_IsInterruptableDueToZoneChange@@YI_NPAUS@@@Z
struct S; typedef int (__fastcall* VFn)(S*);
struct S { VFn* vtbl; };
bool __fastcall CCombatSequenceBase_IsInterruptableDueToZoneChange(S* sequence)
{
    return sequence->vtbl[0x10/4](sequence) == 2;
}