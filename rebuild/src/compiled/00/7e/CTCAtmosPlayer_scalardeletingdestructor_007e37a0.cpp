struct CTCAtmosPlayer {
    void* vftable;
};

void __fastcall CTCAtmosPlayer_dtor(CTCAtmosPlayer* player);
void operator delete(void*);

void* __fastcall CTCAtmosPlayer_scalar_deleting_destructor(CTCAtmosPlayer* player, int edx_unused, unsigned int flags)
{
    CTCAtmosPlayer_dtor(player);
    if (flags & 1)
        operator delete(player);
    return player;
}