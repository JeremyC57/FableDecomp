#pragma optimize("s", on)
extern void __fastcall CTCKrakenTentacle_Destruct(void* tentacle);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCKrakenTentacle_ScalarDeletingDestructor(void* tentacle, int, unsigned int flags)
{
    CTCKrakenTentacle_Destruct(tentacle);
    if (flags & 1)
        OperatorDelete(tentacle);
    return tentacle;
}