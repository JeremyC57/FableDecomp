struct CActiveSoundInfo {
    void* vftable;
};

void __fastcall CActiveSoundInfo_dtor(CActiveSoundInfo* info);
void operator delete(void*);

void* __fastcall CActiveSoundInfo_scalar_deleting_destructor(CActiveSoundInfo* info, int edx_unused, unsigned int flags)
{
    CActiveSoundInfo_dtor(info);
    if (flags & 1)
        operator delete(info);
    return info;
}