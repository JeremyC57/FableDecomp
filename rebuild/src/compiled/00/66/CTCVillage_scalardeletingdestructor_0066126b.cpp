#pragma optimize("s", on)
extern void __fastcall CTCVillage_Destruct(void* village);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCVillage_ScalarDeletingDestructor(void* village, int, unsigned int flags)
{
    CTCVillage_Destruct(village);
    if (flags & 1)
        OperatorDelete(village);
    return village;
}