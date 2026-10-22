struct CModeGuildmasterPowerUpSpeed {
    void* vftable;
};

void __fastcall CModeGuildmasterPowerUpSpeed_dtor(CModeGuildmasterPowerUpSpeed* mode);
void operator delete(void*);

void* __fastcall CModeGuildmasterPowerUpSpeed_scalar_deleting_destructor(CModeGuildmasterPowerUpSpeed* mode, int edx_unused, unsigned int flags)
{
    CModeGuildmasterPowerUpSpeed_dtor(mode);
    if (flags & 1)
        operator delete(mode);
    return mode;
}