#pragma optimize("s", on)
extern void __fastcall CTCInGameMenu_Destruct(void* menu);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCInGameMenu_ScalarDeletingDestructor(void* menu, int, unsigned int flags)
{
    CTCInGameMenu_Destruct(menu);
    if (flags & 1)
        OperatorDelete(menu);
    return menu;
}