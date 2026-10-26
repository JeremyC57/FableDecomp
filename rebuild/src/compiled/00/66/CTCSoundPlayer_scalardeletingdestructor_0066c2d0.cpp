class CTCSoundPlayer { public:
    void* vftable;
};

void __fastcall CTCSoundPlayer_dtor(CTCSoundPlayer* player);
void operator delete(void*);

void* __fastcall CTCSoundPlayer_scalar_deleting_destructor(CTCSoundPlayer* player, int edx_unused, unsigned int flags)
{
    CTCSoundPlayer_dtor(player);
    if (flags & 1)
        operator delete(player);
    return player;
}