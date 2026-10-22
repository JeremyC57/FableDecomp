#pragma optimize("s", on)
extern void __fastcall CTCDHeroSuit_Destruct(void* suit);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCDHeroSuit_ScalarDeletingDestructor(void* suit, int, unsigned int flags)
{
    CTCDHeroSuit_Destruct(suit);
    if (flags & 1)
        OperatorDelete(suit);
    return suit;
}