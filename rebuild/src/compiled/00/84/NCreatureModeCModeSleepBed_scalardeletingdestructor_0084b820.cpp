struct CModeSleepBed {
    void* vftable;
};

void __fastcall CModeSleepBed_dtor(CModeSleepBed* mode);
void operator delete(void*);

void* __fastcall CModeSleepBed_scalar_deleting_destructor(CModeSleepBed* mode, int edx_unused, unsigned int flags)
{
    CModeSleepBed_dtor(mode);
    if (flags & 1)
        operator delete(mode);
    return mode;
}