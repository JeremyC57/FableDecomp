#pragma optimize("s", on)
extern void __fastcall CTCTargeted_Destruct(void* targeted);
void* __fastcall CTCTargeted_ScalarDeletingDestructor(void* targeted, int, unsigned int flags)
{
    CTCTargeted_Destruct(targeted);
    if (flags & 1)
        operator delete(targeted);
    return targeted;
}