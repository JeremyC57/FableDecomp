#pragma optimize("s", on)
extern void __fastcall CTCInventoryAbilities_Destruct(void* abilities);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCInventoryAbilities_ScalarDeletingDestructor(void* abilities, int, unsigned int flags)
{
    CTCInventoryAbilities_Destruct(abilities);
    if (flags & 1)
        OperatorDelete(abilities);
    return abilities;
}