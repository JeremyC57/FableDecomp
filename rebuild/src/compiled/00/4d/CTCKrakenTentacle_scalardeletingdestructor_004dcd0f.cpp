#pragma optimize("s", on)
extern void __fastcall CTCKrakenTentacle_Destruct(void* tentacle);
void* __fastcall CTCKrakenTentacle_ScalarDeletingDestructor(void* tentacle, int, unsigned int flags)
{
    CTCKrakenTentacle_Destruct(tentacle);
    if (flags & 1)
        operator delete(tentacle);
    return tentacle;
}