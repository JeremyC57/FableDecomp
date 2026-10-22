struct CInputProcessFirstPerson {
    void* vftable;
};

void __fastcall CInputProcessFirstPerson_dtor(CInputProcessFirstPerson* process);
void operator delete(void*);

void* __fastcall CInputProcessFirstPerson_scalar_deleting_destructor(CInputProcessFirstPerson* process, int edx_unused, unsigned int flags)
{
    CInputProcessFirstPerson_dtor(process);
    if (flags & 1)
        operator delete(process);
    return process;
}