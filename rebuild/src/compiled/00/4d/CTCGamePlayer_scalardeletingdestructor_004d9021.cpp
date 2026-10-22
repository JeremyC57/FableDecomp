#pragma optimize("s", on)
extern void __fastcall DestructMember(void* member);
extern void __fastcall CTCGamePlayer_Destruct(void* player);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCGamePlayer_CompositeScalarDeletingDestructor(void* player, int, unsigned int flags)
{
    DestructMember(static_cast<unsigned char*>(player) + 16);
    CTCGamePlayer_Destruct(player);
    if (flags & 1)
        OperatorDelete(player);
    return player;
}