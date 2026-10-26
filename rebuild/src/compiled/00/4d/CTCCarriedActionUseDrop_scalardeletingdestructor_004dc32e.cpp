#pragma optimize("s", on)
extern void __fastcall CTCCarriedActionUseDrop_Destruct(void* drop);
void* __fastcall CTCCarriedActionUseDrop_ScalarDeletingDestructor(void* drop, int, unsigned int flags)
{
    CTCCarriedActionUseDrop_Destruct(drop);
    if (flags & 1)
        operator delete(drop);
    return drop;
}