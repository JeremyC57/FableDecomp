struct CModeIceTrollGroundSmashIcicleAssault {
    void* vftable;
};

void __fastcall CModeIceTrollGroundSmashIcicleAssault_dtor(CModeIceTrollGroundSmashIcicleAssault* mode);
void operator delete(void*);

void* __fastcall CModeIceTrollGroundSmashIcicleAssault_scalar_deleting_destructor(CModeIceTrollGroundSmashIcicleAssault* mode, int edx_unused, unsigned int flags)
{
    CModeIceTrollGroundSmashIcicleAssault_dtor(mode);
    if (flags & 1)
        operator delete(mode);
    return mode;
}