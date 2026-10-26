#pragma optimize("s", on)
extern void __fastcall CTCVillage_Destruct(void* village);
void* __fastcall CTCVillage_ScalarDeletingDestructor(void* village, int, unsigned int flags)
{
    CTCVillage_Destruct(village);
    if (flags & 1)
        operator delete(village);
    return village;
}