#pragma optimize("s", on)
extern void __fastcall CTCActionUseTorch_Destruct(void* torch);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCActionUseTorch_ScalarDeletingDestructor(void* torch, int, unsigned int flags)
{
    CTCActionUseTorch_Destruct(torch);
    if (flags & 1)
        OperatorDelete(torch);
    return torch;
}