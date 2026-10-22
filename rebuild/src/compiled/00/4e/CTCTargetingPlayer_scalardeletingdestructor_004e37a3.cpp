#pragma optimize("s", on)
extern void __fastcall CTCTargetingPlayer_Destruct(void* player);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCTargetingPlayer_ScalarDeletingDestructor(void* player, int, unsigned int flags)
{
    CTCTargetingPlayer_Destruct(player);
    if (flags & 1)
        OperatorDelete(player);
    return player;
}