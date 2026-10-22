#pragma optimize("s", on)
extern void __fastcall CTCWobble_Destruct(void* wobble);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCWobble_ScalarDeletingDestructor(void* wobble, int, unsigned int flags)
{
    CTCWobble_Destruct(wobble);
    if (flags & 1)
        OperatorDelete(wobble);
    return wobble;
}