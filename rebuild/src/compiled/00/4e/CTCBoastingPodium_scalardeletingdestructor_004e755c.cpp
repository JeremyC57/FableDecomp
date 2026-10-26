#pragma optimize("s", on)
extern void __fastcall CTCBoastingPodium_Destruct(void* podium);
void* __fastcall CTCBoastingPodium_ScalarDeletingDestructor(void* podium, int, unsigned int flags)
{
    CTCBoastingPodium_Destruct(podium);
    if (flags & 1)
        operator delete(podium);
    return podium;
}