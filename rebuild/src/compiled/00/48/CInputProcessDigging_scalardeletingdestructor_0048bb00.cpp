class CInputProcessDigging { public:
    void* vftable;
};

void __fastcall CInputProcessDigging_dtor(CInputProcessDigging* process);
void operator delete(void*);

void* __fastcall CInputProcessDigging_scalar_deleting_destructor(CInputProcessDigging* process, int edx_unused, unsigned int flags)
{
    CInputProcessDigging_dtor(process);
    if (flags & 1)
        operator delete(process);
    return process;
}