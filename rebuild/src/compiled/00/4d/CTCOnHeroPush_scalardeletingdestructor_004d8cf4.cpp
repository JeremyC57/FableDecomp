#pragma optimize("s", on)
extern void __fastcall CTCOnHeroPush_Destruct(void* push);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCOnHeroPush_ScalarDeletingDestructor(void* push, int, unsigned int flags)
{
    CTCOnHeroPush_Destruct(push);
    if (flags & 1)
        OperatorDelete(push);
    return push;
}