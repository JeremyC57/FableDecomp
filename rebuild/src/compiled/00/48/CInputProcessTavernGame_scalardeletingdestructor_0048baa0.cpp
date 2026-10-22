struct CInputProcessTavernGame {
    void* vftable;
};

void __fastcall CInputProcessTavernGame_dtor(CInputProcessTavernGame* process);
void operator delete(void*);

void* __fastcall CInputProcessTavernGame_scalar_deleting_destructor(CInputProcessTavernGame* process, int edx_unused, unsigned int flags)
{
    CInputProcessTavernGame_dtor(process);
    if (flags & 1)
        operator delete(process);
    return process;
}