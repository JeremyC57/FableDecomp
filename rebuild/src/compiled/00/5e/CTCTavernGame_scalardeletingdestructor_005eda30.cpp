class CTCTavernGame { public:
    void* vftable;
};

void __fastcall CTCTavernGame_dtor(CTCTavernGame* game);
void operator delete(void*);

void* __fastcall CTCTavernGame_scalar_deleting_destructor(CTCTavernGame* game, int edx_unused, unsigned int flags)
{
    CTCTavernGame_dtor(game);
    if (flags & 1)
        operator delete(game);
    return game;
}