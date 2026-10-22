struct CAInputProcess {
    void* vftable;
};

void __fastcall CAInputProcess_dtor(CAInputProcess* process);
void operator delete(void*);

void* __fastcall CAInputProcess_scalar_deleting_destructor(CAInputProcess* process, int edx_unused, unsigned int flags)
{
    CAInputProcess_dtor(process);
    if (flags & 1)
        operator delete(process);
    return process;
}