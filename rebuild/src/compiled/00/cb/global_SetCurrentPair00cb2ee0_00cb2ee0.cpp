// ImeUi_SetScreenDimension(unsigned int, unsigned int) -- stores the screen size when ImeUi is initialised
// Leaf: ?ImeUi_SetScreenDimension@@YIXII@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CB2EE0, length 22 bytes (+ int3 padding).
// Identity (Codex pass 12 respell, 2026-11-03): Ego_r.pdb 0x00C94D30 + FableWin.pdb 0x03581520 ?ImeUi_SetScreenDimension@@YIXII@Z
// (DXUT ImeUi.cpp); the landed caller NUISystem::CIME::Initialise 0x00405650 already binds 0x00CB2EE0 with that symbol, and
// landed DrawImeIndicator 0x00CB2F80 reads 0x0143CD54 / 0x0143CD60 as g_screenWidth / g_screenHeight. It was landed as the
// descriptive SetCurrentPair_00cb2ee0 over g_pairEnabled_0143e8ae / g_currentFirst_0143cd54 / g_currentSecond_0143cd60.
extern bool         g_bInitialized;                 // 0x0143E8AE
extern unsigned int g_screenWidth;                  // 0x0143CD54
extern unsigned int g_screenHeight;                 // 0x0143CD60

void __fastcall ImeUi_SetScreenDimension(unsigned int width, unsigned int height)
{
    if (!g_bInitialized)
        return;
    g_screenWidth = width;
    g_screenHeight = height;
}
