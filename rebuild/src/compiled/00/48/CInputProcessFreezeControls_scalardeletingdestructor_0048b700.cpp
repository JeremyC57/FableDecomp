struct CInputProcessFreezeControls {
    void* vftable;
};

void __fastcall CInputProcessFreezeControls_dtor(CInputProcessFreezeControls* process);
void operator delete(void*);

void* __fastcall CInputProcessFreezeControls_scalar_deleting_destructor(CInputProcessFreezeControls* process, int edx_unused, unsigned int flags)
{
    CInputProcessFreezeControls_dtor(process);
    if (flags & 1)
        operator delete(process);
    return process;
}