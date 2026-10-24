struct CDisplayEngine;
// Leaf: GetPlayerManager
struct CGlobalHolder { CDisplayEngine* engine; };
extern CGlobalHolder* g_displayGlobal;
struct CViewBase {};

CDisplayEngine* __fastcall CViewBase_GetDisplayEngine(const CViewBase* self)
{
    (void)self;
    return *(CDisplayEngine**)((char*)g_displayGlobal + 0x30);
}