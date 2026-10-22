struct CASuspendableProcess {
    void* vftable;
};

void __fastcall CASuspendableProcess_dtor(CASuspendableProcess* process);
void operator delete(void*);

void* __fastcall CASuspendableProcess_scalar_deleting_destructor(CASuspendableProcess* process, int edx_unused, unsigned int flags)
{
    CASuspendableProcess_dtor(process);
    if (flags & 1)
        operator delete(process);
    return process;
}