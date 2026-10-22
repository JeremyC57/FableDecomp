#pragma optimize("s", on)
extern void __fastcall CTCHasName_Destruct(void* name);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCHasName_ScalarDeletingDestructor(void* name, int, unsigned int flags)
{
    CTCHasName_Destruct(name);
    if (flags & 1)
        OperatorDelete(name);
    return name;
}