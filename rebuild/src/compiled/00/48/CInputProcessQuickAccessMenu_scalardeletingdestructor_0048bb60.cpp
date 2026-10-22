struct CInputProcessQuickAccessMenu {
    void* vftable;
};

void __fastcall CInputProcessQuickAccessMenu_dtor(CInputProcessQuickAccessMenu* process);
void operator delete(void*);

void* __fastcall CInputProcessQuickAccessMenu_scalar_deleting_destructor(CInputProcessQuickAccessMenu* process, int edx_unused, unsigned int flags)
{
    CInputProcessQuickAccessMenu_dtor(process);
    if (flags & 1)
        operator delete(process);
    return process;
}