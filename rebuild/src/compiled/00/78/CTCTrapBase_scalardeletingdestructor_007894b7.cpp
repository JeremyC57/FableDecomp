#pragma optimize("s", on)
extern void __fastcall CTCTrapBase_Destruct(void* trap);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCTrapBase_ScalarDeletingDestructor(void* trap, int, unsigned int flags)
{
    CTCTrapBase_Destruct(trap);
    if (flags & 1)
        OperatorDelete(trap);
    return trap;
}