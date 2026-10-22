#pragma optimize("s", on)
extern void __fastcall CTCTargeted_Destruct(void* targeted);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCTargeted_ScalarDeletingDestructor(void* targeted, int, unsigned int flags)
{
    CTCTargeted_Destruct(targeted);
    if (flags & 1)
        OperatorDelete(targeted);
    return targeted;
}