#pragma optimize("s", on)
extern void __fastcall CTCBoastingPodium_Destruct(void* podium);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCBoastingPodium_ScalarDeletingDestructor(void* podium, int, unsigned int flags)
{
    CTCBoastingPodium_Destruct(podium);
    if (flags & 1)
        OperatorDelete(podium);
    return podium;
}