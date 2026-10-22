#pragma optimize("s", on)
extern void __fastcall CTCDestroyThingOnSpecialAbilityFinish_Destruct(void* finish);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCDestroyThingOnSpecialAbilityFinish_ScalarDeletingDestructor(void* finish, int, unsigned int flags)
{
    CTCDestroyThingOnSpecialAbilityFinish_Destruct(finish);
    if (flags & 1)
        OperatorDelete(finish);
    return finish;
}