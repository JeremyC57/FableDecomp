class CInputProcessZTarget { public:
    void* vftable;
};

void __fastcall CInputProcessZTarget_dtor(CInputProcessZTarget* process);
void operator delete(void*);

void* __fastcall CInputProcessZTarget_scalar_deleting_destructor(CInputProcessZTarget* process, int edx_unused, unsigned int flags)
{
    CInputProcessZTarget_dtor(process);
    if (flags & 1)
        operator delete(process);
    return process;
}