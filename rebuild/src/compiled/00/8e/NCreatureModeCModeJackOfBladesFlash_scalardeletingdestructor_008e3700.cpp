struct CModeJackOfBladesFlash {
    void* vftable;
};

void __fastcall CModeJackOfBladesFlash_dtor(CModeJackOfBladesFlash* mode);
void operator delete(void*);

void* __fastcall CModeJackOfBladesFlash_scalar_deleting_destructor(CModeJackOfBladesFlash* mode, int edx_unused, unsigned int flags)
{
    CModeJackOfBladesFlash_dtor(mode);
    if (flags & 1)
        operator delete(mode);
    return mode;
}