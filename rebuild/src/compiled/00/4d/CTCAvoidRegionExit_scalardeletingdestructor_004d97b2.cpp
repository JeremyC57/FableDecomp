#pragma optimize("s", on)
extern void __fastcall CTCAvoidRegionExit_Destruct(void* exit);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCAvoidRegionExit_ScalarDeletingDestructor(void* exit, int, unsigned int flags)
{
    CTCAvoidRegionExit_Destruct(exit);
    if (flags & 1)
        OperatorDelete(exit);
    return exit;
}