struct CInputProcessOracleMinigame {
    void* vftable;
};

void __fastcall CInputProcessOracleMinigame_dtor(CInputProcessOracleMinigame* process);
void operator delete(void*);

void* __fastcall CInputProcessOracleMinigame_scalar_deleting_destructor(CInputProcessOracleMinigame* process, int edx_unused, unsigned int flags)
{
    CInputProcessOracleMinigame_dtor(process);
    if (flags & 1)
        operator delete(process);
    return process;
}