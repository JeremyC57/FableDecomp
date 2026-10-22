struct CInputProcessParalysed {
    void* vftable;
};

void __fastcall CInputProcessParalysed_dtor(CInputProcessParalysed* process);
void operator delete(void*);

void* __fastcall CInputProcessParalysed_scalar_deleting_destructor(CInputProcessParalysed* process, int edx_unused, unsigned int flags)
{
    CInputProcessParalysed_dtor(process);
    if (flags & 1)
        operator delete(process);
    return process;
}