struct CInputProcessFishing {
    void* vftable;
};

void __fastcall CInputProcessFishing_dtor(CInputProcessFishing* process);
void operator delete(void*);

void* __fastcall CInputProcessFishing_scalar_deleting_destructor(CInputProcessFishing* process, int edx_unused, unsigned int flags)
{
    CInputProcessFishing_dtor(process);
    if (flags & 1)
        operator delete(process);
    return process;
}