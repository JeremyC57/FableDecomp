class CInputProcessUseEnvironment { public:
    void* vftable;
};

void __fastcall CInputProcessUseEnvironment_dtor(CInputProcessUseEnvironment* process);
void operator delete(void*);

void* __fastcall CInputProcessUseEnvironment_scalar_deleting_destructor(CInputProcessUseEnvironment* process, int edx_unused, unsigned int flags)
{
    CInputProcessUseEnvironment_dtor(process);
    if (flags & 1)
        operator delete(process);
    return process;
}