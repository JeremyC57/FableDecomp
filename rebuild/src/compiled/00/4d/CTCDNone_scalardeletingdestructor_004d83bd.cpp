#pragma optimize("s", on)
extern void __fastcall CTCDNone_Destruct(void* none);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCDNone_ScalarDeletingDestructor(void* none, int, unsigned int flags)
{
    CTCDNone_Destruct(none);
    if (flags & 1)
        OperatorDelete(none);
    return none;
}