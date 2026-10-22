struct CInputProcessFireheartMinigame {
    void* vftable;
};

void __fastcall CInputProcessFireheartMinigame_dtor(CInputProcessFireheartMinigame* process);
void operator delete(void*);

void* __fastcall CInputProcessFireheartMinigame_scalar_deleting_destructor(CInputProcessFireheartMinigame* process, int edx_unused, unsigned int flags)
{
    CInputProcessFireheartMinigame_dtor(process);
    if (flags & 1)
        operator delete(process);
    return process;
}