struct CInputProcessCameraLookAround {
    void* vftable;
};

void __fastcall CInputProcessCameraLookAround_dtor(CInputProcessCameraLookAround* process);
void operator delete(void*);

void* __fastcall CInputProcessCameraLookAround_scalar_deleting_destructor(CInputProcessCameraLookAround* process, int edx_unused, unsigned int flags)
{
    CInputProcessCameraLookAround_dtor(process);
    if (flags & 1)
        operator delete(process);
    return process;
}