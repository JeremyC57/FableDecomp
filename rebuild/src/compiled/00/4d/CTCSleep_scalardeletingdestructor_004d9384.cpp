#pragma optimize("s", on)
extern void __fastcall CTCSleep_Destruct(void* sleep);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCSleep_ScalarDeletingDestructor(void* sleep, int, unsigned int flags)
{
    CTCSleep_Destruct(sleep);
    if (flags & 1)
        OperatorDelete(sleep);
    return sleep;
}