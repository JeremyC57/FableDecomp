struct COrbitGame {
    void* vftable;
};

void __fastcall COrbitGame_dtor(COrbitGame* game);
void operator delete(void*);

void* __fastcall COrbitGame_scalar_deleting_destructor(COrbitGame* game, int edx_unused, unsigned int flags)
{
    COrbitGame_dtor(game);
    if (flags & 1)
        operator delete(game);
    return game;
}