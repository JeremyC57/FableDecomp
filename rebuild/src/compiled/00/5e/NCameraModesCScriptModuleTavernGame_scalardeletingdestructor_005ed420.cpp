struct CScriptModuleTavernGame {
    void* vftable;
};

void __fastcall CScriptModuleTavernGame_dtor(CScriptModuleTavernGame* game);
void operator delete(void*);

void* __fastcall CScriptModuleTavernGame_scalar_deleting_destructor(CScriptModuleTavernGame* game, int edx_unused, unsigned int flags)
{
    CScriptModuleTavernGame_dtor(game);
    if (flags & 1)
        operator delete(game);
    return game;
}