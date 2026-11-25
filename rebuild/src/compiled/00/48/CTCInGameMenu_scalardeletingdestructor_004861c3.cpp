#pragma optimize("s", on)
// Naming (NF423 gen, 2026-11-25): the callee 0x0047E07F now binds its PDB spelling ??1CTCInGameMenu@@UAE@XZ (the T1 tracker target; the self row's
//   Leaf / PDB name sets), called qualified on a declaration of the class (no body, no layout); was the free stand-in ?CTCInGameMenu_Destruct@@YIXPAX@Z.
class CTCInGameMenu { public: virtual ~CTCInGameMenu(); };
void* __fastcall CTCInGameMenu_ScalarDeletingDestructor(void* menu, int, unsigned int flags)
{
    reinterpret_cast<CTCInGameMenu*>(menu)->CTCInGameMenu::~CTCInGameMenu();
    if (flags & 1)
        operator delete(menu);
    return menu;
}