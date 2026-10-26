#pragma optimize("s", on)
extern void __fastcall CTCCarriedActionUseFish_Destruct(void* fish);
void* __fastcall CTCCarriedActionUseFish_ScalarDeletingDestructor(void* fish, int, unsigned int flags)
{
    CTCCarriedActionUseFish_Destruct(fish);
    if (flags & 1)
        operator delete(fish);
    return fish;
}