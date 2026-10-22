struct CModeThunderLightningStorm {
    void* vftable;
};

void __fastcall CModeThunderLightningStorm_dtor(CModeThunderLightningStorm* mode);
void operator delete(void*);

void* __fastcall CModeThunderLightningStorm_scalar_deleting_destructor(CModeThunderLightningStorm* mode, int edx_unused, unsigned int flags)
{
    CModeThunderLightningStorm_dtor(mode);
    if (flags & 1)
        operator delete(mode);
    return mode;
}