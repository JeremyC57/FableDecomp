#pragma optimize("s", on)
extern void __fastcall CTCDestroyThingOnActionFinish_Destruct(void* finish);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCDestroyThingOnActionFinish_ScalarDeletingDestructor(void* finish, int, unsigned int flags)
{
    CTCDestroyThingOnActionFinish_Destruct(finish);
    if (flags & 1)
        OperatorDelete(finish);
    return finish;
}