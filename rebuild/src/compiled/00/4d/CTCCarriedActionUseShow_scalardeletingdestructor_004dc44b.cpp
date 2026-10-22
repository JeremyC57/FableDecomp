#pragma optimize("s", on)
extern void __fastcall CTCCarriedActionUseShow_Destruct(void* show);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCCarriedActionUseShow_ScalarDeletingDestructor(void* show, int, unsigned int flags)
{
    CTCCarriedActionUseShow_Destruct(show);
    if (flags & 1)
        OperatorDelete(show);
    return show;
}