struct CInputProcessFirstPersonTargeting {
    void* vftable;
};

void __fastcall CInputProcessFirstPersonTargeting_dtor(CInputProcessFirstPersonTargeting* process);
void operator delete(void*);

void* __fastcall CInputProcessFirstPersonTargeting_scalar_deleting_destructor(CInputProcessFirstPersonTargeting* process, int edx_unused, unsigned int flags)
{
    CInputProcessFirstPersonTargeting_dtor(process);
    if (flags & 1)
        operator delete(process);
    return process;
}