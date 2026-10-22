struct CSoundManagerLH {
    void* vftable;
};

void __fastcall CSoundManagerLH_dtor(CSoundManagerLH* managerLH);
void operator delete(void*);

void* __fastcall CSoundManagerLH_scalar_deleting_destructor(CSoundManagerLH* managerLH, int edx_unused, unsigned int flags)
{
    CSoundManagerLH_dtor(managerLH);
    if (flags & 1)
        operator delete(managerLH);
    return managerLH;
}