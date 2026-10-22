struct CModeSummonerSummon {
    void* vftable;
};

void __fastcall CModeSummonerSummon_dtor(CModeSummonerSummon* mode);
void operator delete(void*);

void* __fastcall CModeSummonerSummon_scalar_deleting_destructor(CModeSummonerSummon* mode, int edx_unused, unsigned int flags)
{
    CModeSummonerSummon_dtor(mode);
    if (flags & 1)
        operator delete(mode);
    return mode;
}