#pragma optimize("s", on)
extern void __fastcall CTCStealableItemLocation_Destruct(void* location);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCStealableItemLocation_ScalarDeletingDestructor(void* location, int, unsigned int flags)
{
    CTCStealableItemLocation_Destruct(location);
    if (flags & 1)
        OperatorDelete(location);
    return location;
}