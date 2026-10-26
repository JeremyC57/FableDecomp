#pragma optimize("s", on)
extern void __fastcall CTCCarriedActionUseFishingRod_Destruct(void* rod);
void* __fastcall CTCCarriedActionUseFishingRod_ScalarDeletingDestructor(void* rod, int, unsigned int flags)
{
    CTCCarriedActionUseFishingRod_Destruct(rod);
    if (flags & 1)
        operator delete(rod);
    return rod;
}