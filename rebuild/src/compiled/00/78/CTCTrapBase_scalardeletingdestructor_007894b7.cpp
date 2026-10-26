#pragma optimize("s", on)
extern void __fastcall CTCTrapBase_Destruct(void* trap);
void* __fastcall CTCTrapBase_ScalarDeletingDestructor(void* trap, int, unsigned int flags)
{
    CTCTrapBase_Destruct(trap);
    if (flags & 1)
        operator delete(trap);
    return trap;
}