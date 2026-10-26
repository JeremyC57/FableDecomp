#pragma optimize("s", on)
extern void __fastcall CTCActionUseBlowHorn_Destruct(void* horn);
void* __fastcall CTCActionUseBlowHorn_ScalarDeletingDestructor(void* horn, int, unsigned int flags)
{
    CTCActionUseBlowHorn_Destruct(horn);
    if (flags & 1)
        operator delete(horn);
    return horn;
}