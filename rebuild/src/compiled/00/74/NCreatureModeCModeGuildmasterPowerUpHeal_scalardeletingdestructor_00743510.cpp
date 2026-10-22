struct CModeGuildmasterPowerUpHeal {
    void* vftable;
};

void __fastcall CModeGuildmasterPowerUpHeal_dtor(CModeGuildmasterPowerUpHeal* mode);
void operator delete(void*);

void* __fastcall CModeGuildmasterPowerUpHeal_scalar_deleting_destructor(CModeGuildmasterPowerUpHeal* mode, int edx_unused, unsigned int flags)
{
    CModeGuildmasterPowerUpHeal_dtor(mode);
    if (flags & 1)
        operator delete(mode);
    return mode;
}