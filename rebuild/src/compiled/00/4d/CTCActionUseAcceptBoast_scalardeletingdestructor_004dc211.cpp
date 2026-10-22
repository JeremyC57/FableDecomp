#pragma optimize("s", on)
extern void __fastcall CTCActionUseAcceptBoast_Destruct(void* boast);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCActionUseAcceptBoast_ScalarDeletingDestructor(void* boast, int, unsigned int flags)
{
    CTCActionUseAcceptBoast_Destruct(boast);
    if (flags & 1)
        OperatorDelete(boast);
    return boast;
}