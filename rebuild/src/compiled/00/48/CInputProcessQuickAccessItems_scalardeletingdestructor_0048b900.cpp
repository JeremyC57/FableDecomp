class CInputProcessQuickAccessItems { public:
    void* vftable;
};

void __fastcall CInputProcessQuickAccessItems_dtor(CInputProcessQuickAccessItems* process);
void operator delete(void*);

void* __fastcall CInputProcessQuickAccessItems_scalar_deleting_destructor(CInputProcessQuickAccessItems* process, int edx_unused, unsigned int flags)
{
    CInputProcessQuickAccessItems_dtor(process);
    if (flags & 1)
        operator delete(process);
    return process;
}