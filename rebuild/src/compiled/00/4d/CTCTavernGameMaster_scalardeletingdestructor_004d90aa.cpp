#pragma optimize("s", on)
extern void __fastcall CTCTavernGameMaster_Destruct(void* master);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCTavernGameMaster_ScalarDeletingDestructor(void* master, int, unsigned int flags)
{
    CTCTavernGameMaster_Destruct(master);
    if (flags & 1)
        OperatorDelete(master);
    return master;
}