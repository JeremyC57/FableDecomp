#pragma optimize("s", on)
extern void __fastcall CTCCarriedActionUseDrop_Destruct(void* drop);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCCarriedActionUseDrop_ScalarDeletingDestructor(void* drop, int, unsigned int flags)
{
    CTCCarriedActionUseDrop_Destruct(drop);
    if (flags & 1)
        OperatorDelete(drop);
    return drop;
}