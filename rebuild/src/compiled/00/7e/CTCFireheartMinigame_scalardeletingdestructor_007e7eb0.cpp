struct CTCFireheartMinigame {
    void* vftable;
};

void __fastcall CTCFireheartMinigame_dtor(CTCFireheartMinigame* minigame);
void operator delete(void*);

void* __fastcall CTCFireheartMinigame_scalar_deleting_destructor(CTCFireheartMinigame* minigame, int edx_unused, unsigned int flags)
{
    CTCFireheartMinigame_dtor(minigame);
    if (flags & 1)
        operator delete(minigame);
    return minigame;
}