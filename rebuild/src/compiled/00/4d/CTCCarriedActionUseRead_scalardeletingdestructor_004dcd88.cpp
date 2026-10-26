#pragma optimize("s", on)
extern void __fastcall CTCCarriedActionUseRead_Destruct(void* read);
void* __fastcall CTCCarriedActionUseRead_ScalarDeletingDestructor(void* read, int, unsigned int flags)
{
    CTCCarriedActionUseRead_Destruct(read);
    if (flags & 1)
        operator delete(read);
    return read;
}