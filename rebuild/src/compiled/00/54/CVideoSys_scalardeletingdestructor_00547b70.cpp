struct CVideoSys {
    void* vftable;
};

void __fastcall CVideoSys_dtor(CVideoSys* sys);
void operator delete(void*);

void* __fastcall CVideoSys_scalar_deleting_destructor(CVideoSys* sys, int edx_unused, unsigned int flags)
{
    CVideoSys_dtor(sys);
    if (flags & 1)
        operator delete(sys);
    return sys;
}