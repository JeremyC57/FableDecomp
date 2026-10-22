#pragma optimize("s", on)
extern void __fastcall CTCActionUseBlowHorn_Destruct(void* horn);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCActionUseBlowHorn_ScalarDeletingDestructor(void* horn, int, unsigned int flags)
{
    CTCActionUseBlowHorn_Destruct(horn);
    if (flags & 1)
        OperatorDelete(horn);
    return horn;
}