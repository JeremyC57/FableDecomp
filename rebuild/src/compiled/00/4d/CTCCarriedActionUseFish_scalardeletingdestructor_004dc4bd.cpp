#pragma optimize("s", on)
extern void __fastcall CTCCarriedActionUseFish_Destruct(void* fish);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCCarriedActionUseFish_ScalarDeletingDestructor(void* fish, int, unsigned int flags)
{
    CTCCarriedActionUseFish_Destruct(fish);
    if (flags & 1)
        OperatorDelete(fish);
    return fish;
}