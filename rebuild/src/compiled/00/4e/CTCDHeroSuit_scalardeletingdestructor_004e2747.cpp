#pragma optimize("s", on)
extern void __fastcall CTCDHeroSuit_Destruct(void* suit);
void* __fastcall CTCDHeroSuit_ScalarDeletingDestructor(void* suit, int, unsigned int flags)
{
    CTCDHeroSuit_Destruct(suit);
    if (flags & 1)
        operator delete(suit);
    return suit;
}