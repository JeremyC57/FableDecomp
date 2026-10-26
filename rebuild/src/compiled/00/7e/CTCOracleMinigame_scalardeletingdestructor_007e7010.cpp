class CTCOracleMinigame { public:
    void* vftable;
};

void __fastcall CTCOracleMinigame_dtor(CTCOracleMinigame* minigame);
void operator delete(void*);

void* __fastcall CTCOracleMinigame_scalar_deleting_destructor(CTCOracleMinigame* minigame, int edx_unused, unsigned int flags)
{
    CTCOracleMinigame_dtor(minigame);
    if (flags & 1)
        operator delete(minigame);
    return minigame;
}