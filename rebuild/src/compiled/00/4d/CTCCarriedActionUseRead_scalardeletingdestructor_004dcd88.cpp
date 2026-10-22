#pragma optimize("s", on)
extern void __fastcall CTCCarriedActionUseRead_Destruct(void* read);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCCarriedActionUseRead_ScalarDeletingDestructor(void* read, int, unsigned int flags)
{
    CTCCarriedActionUseRead_Destruct(read);
    if (flags & 1)
        OperatorDelete(read);
    return read;
}