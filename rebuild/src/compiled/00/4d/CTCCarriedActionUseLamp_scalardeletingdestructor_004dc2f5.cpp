#pragma optimize("s", on)
extern void __fastcall CTCCarriedActionUseLamp_Destruct(void* lamp);
void* __fastcall CTCCarriedActionUseLamp_ScalarDeletingDestructor(void* lamp, int, unsigned int flags)
{
    CTCCarriedActionUseLamp_Destruct(lamp);
    if (flags & 1)
        operator delete(lamp);
    return lamp;
}