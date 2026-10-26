class CInputProcessControlFreeCamera { public:
    void* vftable;
};

void __fastcall CInputProcessControlFreeCamera_dtor(CInputProcessControlFreeCamera* process);
void operator delete(void*);

void* __fastcall CInputProcessControlFreeCamera_scalar_deleting_destructor(CInputProcessControlFreeCamera* process, int edx_unused, unsigned int flags)
{
    CInputProcessControlFreeCamera_dtor(process);
    if (flags & 1)
        operator delete(process);
    return process;
}