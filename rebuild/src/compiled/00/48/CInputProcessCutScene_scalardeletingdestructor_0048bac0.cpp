class CInputProcessCutScene { public:
    void* vftable;
};

void __fastcall CInputProcessCutScene_dtor(CInputProcessCutScene* process);
void operator delete(void*);

void* __fastcall CInputProcessCutScene_scalar_deleting_destructor(CInputProcessCutScene* process, int edx_unused, unsigned int flags)
{
    CInputProcessCutScene_dtor(process);
    if (flags & 1)
        operator delete(process);
    return process;
}