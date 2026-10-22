struct CInputProcessBetting {
    void* vftable;
};

void __fastcall CInputProcessBetting_dtor(CInputProcessBetting* process);
void operator delete(void*);

void* __fastcall CInputProcessBetting_scalar_deleting_destructor(CInputProcessBetting* process, int edx_unused, unsigned int flags)
{
    CInputProcessBetting_dtor(process);
    if (flags & 1)
        operator delete(process);
    return process;
}