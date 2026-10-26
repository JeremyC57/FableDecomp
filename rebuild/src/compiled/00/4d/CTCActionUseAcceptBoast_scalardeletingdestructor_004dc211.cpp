#pragma optimize("s", on)
extern void __fastcall CTCActionUseAcceptBoast_Destruct(void* boast);
void* __fastcall CTCActionUseAcceptBoast_ScalarDeletingDestructor(void* boast, int, unsigned int flags)
{
    CTCActionUseAcceptBoast_Destruct(boast);
    if (flags & 1)
        operator delete(boast);
    return boast;
}