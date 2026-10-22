struct CInputProcessGameBase {
    void* vftable;
};

void __fastcall CInputProcessGameBase_dtor(CInputProcessGameBase* process);
void operator delete(void*);

void* __fastcall CInputProcessGameBase_scalar_deleting_destructor(CInputProcessGameBase* process, int edx_unused, unsigned int flags)
{
    CInputProcessGameBase_dtor(process);
    if (flags & 1)
        operator delete(process);
    return process;
}