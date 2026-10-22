#pragma optimize("s", on)
extern void __fastcall CTCMeleeTrap_Destruct(void* trap);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCMeleeTrap_ScalarDeletingDestructor(void* trap, int, unsigned int flags)
{
    CTCMeleeTrap_Destruct(trap);
    if (flags & 1)
        OperatorDelete(trap);
    return trap;
}