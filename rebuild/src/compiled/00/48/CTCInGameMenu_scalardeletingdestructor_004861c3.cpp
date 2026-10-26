#pragma optimize("s", on)
extern void __fastcall CTCInGameMenu_Destruct(void* menu);
void* __fastcall CTCInGameMenu_ScalarDeletingDestructor(void* menu, int, unsigned int flags)
{
    CTCInGameMenu_Destruct(menu);
    if (flags & 1)
        operator delete(menu);
    return menu;
}