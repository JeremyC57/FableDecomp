#pragma optimize("s", on)
extern void __fastcall CTCDecapitation_Destruct(void* decapitation);
void* __fastcall CTCDecapitation_ScalarDeletingDestructor(void* decapitation, int, unsigned int flags)
{
    CTCDecapitation_Destruct(decapitation);
    if (flags & 1)
        operator delete(decapitation);
    return decapitation;
}