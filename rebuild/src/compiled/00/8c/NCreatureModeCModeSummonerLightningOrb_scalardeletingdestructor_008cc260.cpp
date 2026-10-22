struct CModeSummonerLightningOrb {
    void* vftable;
};

void __fastcall CModeSummonerLightningOrb_dtor(CModeSummonerLightningOrb* mode);
void operator delete(void*);

void* __fastcall CModeSummonerLightningOrb_scalar_deleting_destructor(CModeSummonerLightningOrb* mode, int edx_unused, unsigned int flags)
{
    CModeSummonerLightningOrb_dtor(mode);
    if (flags & 1)
        operator delete(mode);
    return mode;
}