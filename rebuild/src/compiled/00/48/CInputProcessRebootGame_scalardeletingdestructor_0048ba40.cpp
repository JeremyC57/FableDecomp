class CInputProcessRebootGame { public:
    void* vftable;
};

void __fastcall CInputProcessRebootGame_dtor(CInputProcessRebootGame* process);
void operator delete(void*);

void* __fastcall CInputProcessRebootGame_scalar_deleting_destructor(CInputProcessRebootGame* process, int edx_unused, unsigned int flags)
{
    CInputProcessRebootGame_dtor(process);
    if (flags & 1)
        operator delete(process);
    return process;
}