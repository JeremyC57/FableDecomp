#pragma optimize("s", on)
extern void __fastcall CTCCupboard_Destruct(void* cupboard);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCCupboard_ScalarDeletingDestructor(void* cupboard, int, unsigned int flags)
{
    CTCCupboard_Destruct(cupboard);
    if (flags & 1)
        OperatorDelete(cupboard);
    return cupboard;
}