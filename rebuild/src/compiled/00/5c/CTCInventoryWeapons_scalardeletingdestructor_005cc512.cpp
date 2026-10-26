#pragma optimize("s", on)
extern void __fastcall CTCInventoryWeapons_Destruct(void* weapons);
void* __fastcall CTCInventoryWeapons_ScalarDeletingDestructor(void* weapons, int, unsigned int flags)
{
    CTCInventoryWeapons_Destruct(weapons);
    if (flags & 1)
        operator delete(weapons);
    return weapons;
}