struct CInputProcessInGameMenu {
    void* vftable;
};

void __fastcall CInputProcessInGameMenu_dtor(CInputProcessInGameMenu* process);
void operator delete(void*);

void* __fastcall CInputProcessInGameMenu_scalar_deleting_destructor(CInputProcessInGameMenu* process, int edx_unused, unsigned int flags)
{
    CInputProcessInGameMenu_dtor(process);
    if (flags & 1)
        operator delete(process);
    return process;
}