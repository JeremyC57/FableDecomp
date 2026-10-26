#pragma optimize("s", on)
extern void __fastcall CTCInventoryAbilities_Destruct(void* abilities);
void* __fastcall CTCInventoryAbilities_ScalarDeletingDestructor(void* abilities, int, unsigned int flags)
{
    CTCInventoryAbilities_Destruct(abilities);
    if (flags & 1)
        operator delete(abilities);
    return abilities;
}