struct CModeGuildmasterPowerUpShield {
    void* vftable;
};

void __fastcall CModeGuildmasterPowerUpShield_dtor(CModeGuildmasterPowerUpShield* mode);
void operator delete(void*);

void* __fastcall CModeGuildmasterPowerUpShield_scalar_deleting_destructor(CModeGuildmasterPowerUpShield* mode, int edx_unused, unsigned int flags)
{
    CModeGuildmasterPowerUpShield_dtor(mode);
    if (flags & 1)
        operator delete(mode);
    return mode;
}