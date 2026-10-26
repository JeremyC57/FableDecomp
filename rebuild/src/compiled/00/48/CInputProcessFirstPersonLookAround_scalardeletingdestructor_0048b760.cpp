class CInputProcessFirstPersonLookAround { public:
    void* vftable;
};

void __fastcall CInputProcessFirstPersonLookAround_dtor(CInputProcessFirstPersonLookAround* process);
void operator delete(void*);

void* __fastcall CInputProcessFirstPersonLookAround_scalar_deleting_destructor(CInputProcessFirstPersonLookAround* process, int edx_unused, unsigned int flags)
{
    CInputProcessFirstPersonLookAround_dtor(process);
    if (flags & 1)
        operator delete(process);
    return process;
}