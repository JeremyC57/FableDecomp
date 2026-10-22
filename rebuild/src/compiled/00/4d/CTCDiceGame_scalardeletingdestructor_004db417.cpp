#pragma optimize("s", on)
extern void __fastcall CTCDiceGame_Destruct(void* game);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCDiceGame_ScalarDeletingDestructor(void* game, int, unsigned int flags)
{
    CTCDiceGame_Destruct(game);
    if (flags & 1)
        OperatorDelete(game);
    return game;
}