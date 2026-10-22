#pragma optimize("s", on)
extern void __fastcall CTCCarriedActionUseLamp_Destruct(void* lamp);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCCarriedActionUseLamp_ScalarDeletingDestructor(void* lamp, int, unsigned int flags)
{
    CTCCarriedActionUseLamp_Destruct(lamp);
    if (flags & 1)
        OperatorDelete(lamp);
    return lamp;
}