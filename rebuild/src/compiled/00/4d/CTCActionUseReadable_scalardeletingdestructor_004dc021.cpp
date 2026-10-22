#pragma optimize("s", on)
extern void __fastcall CTCActionUseReadable_Destruct(void* readable);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCActionUseReadable_ScalarDeletingDestructor(void* readable, int, unsigned int flags)
{
    CTCActionUseReadable_Destruct(readable);
    if (flags & 1)
        OperatorDelete(readable);
    return readable;
}