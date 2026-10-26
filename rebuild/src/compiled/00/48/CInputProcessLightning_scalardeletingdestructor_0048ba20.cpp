class CInputProcessLightning { public:
    void* vftable;
};

void __fastcall CInputProcessLightning_dtor(CInputProcessLightning* process);
void operator delete(void*);

void* __fastcall CInputProcessLightning_scalar_deleting_destructor(CInputProcessLightning* process, int edx_unused, unsigned int flags)
{
    CInputProcessLightning_dtor(process);
    if (flags & 1)
        operator delete(process);
    return process;
}