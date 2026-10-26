#pragma optimize("s", on)
extern void __fastcall CTCMeleeTrap_Destruct(void* trap);
void* __fastcall CTCMeleeTrap_ScalarDeletingDestructor(void* trap, int, unsigned int flags)
{
    CTCMeleeTrap_Destruct(trap);
    if (flags & 1)
        operator delete(trap);
    return trap;
}