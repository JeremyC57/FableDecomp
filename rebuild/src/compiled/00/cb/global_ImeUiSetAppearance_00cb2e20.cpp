// ImeUi_SetAppearance(const IMEUI_APPEARANCE* pia) -- DXUT ImeUi.cpp: copy the caller's IME skin, strip the alpha byte of the symbol and composition colours, and recompute the composition colours
// Leaf: ?ImeUi_SetAppearance@@YIXPBUIMEUI_APPEARANCE@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CB2E20, 81 bytes. __fastcall (pia in ecx); FableWin.pdb
// ?ImeUi_SetAppearance@@YIXPBUIMEUI_APPEARANCE@@@Z. The manifest label "SetCurrentInterpolateInfo" is propagated.
// Callee: SetCompStringColor @ 0x00CB2D20 (tail `jmp`).
// Globals: g_bInitialized 0x0143E8AE, gSkinIME 0x013AC708.
// Shape: the DXUT body verbatim -- the struct copy is `rep movsd` x0x10, each `&= 0xffffff` is a byte store of 0
// to the colour's top byte (sharing the zero eax of the two early-out tests).
// Model: DXUT ImeUi.cpp (DirectX SDK "ImeUi" IME library, UNICODE build) as linked into Fable.exe; FableWin.pdb
// names its functions (`?ImeUi_*@@YI...` = __fastcall: the game TU is /Gr, so every plain function and function-
// pointer type defaults to __fastcall, while the WINAPI ones stay __stdcall). The file-static globals are declared
// extern here with their retail addresses; the imm32 calls go through the linker import thunks (not dllimport).
struct HWND__ { int unused; };   typedef HWND__* HWND;
struct HIMC__ { int unused; };   typedef HIMC__* HIMC;
struct HKL__  { int unused; };   typedef HKL__*  HKL;
struct tagRECT { long left, top, right, bottom; };   typedef tagRECT RECT;
class CImeUiFont_Base;

struct IMEUI_APPEARANCE {                           // ImeUi.h, 0x40 bytes
    unsigned long    symbolColor;                   // +0x00
    unsigned long    symbolColorOff;                // +0x04
    unsigned long    symbolColorText;               // +0x08
    unsigned char    symbolHeight;                  // +0x0c
    unsigned char    symbolTranslucence;            // +0x0d
    unsigned char    symbolPlacement;               // +0x0e
    CImeUiFont_Base* symbolFont;                    // +0x10
    unsigned long    candColorBase;                 // +0x14
    unsigned long    candColorBorder;               // +0x18
    unsigned long    candColorText;                 // +0x1c
    unsigned long    compColorInput;                // +0x20
    unsigned long    compColorTargetConv;           // +0x24
    unsigned long    compColorConverted;            // +0x28
    unsigned long    compColorTargetNotConv;        // +0x2c
    unsigned long    compColorInputErr;             // +0x30
    unsigned char    compTranslucence;              // +0x34
    unsigned long    compColorText;                 // +0x38
    unsigned char    caretWidth;                    // +0x3c
    unsigned char    caretYMargin;                  // +0x3d
};

extern bool             g_bInitialized;             // 0x0143E8AE
extern IMEUI_APPEARANCE gSkinIME;                   // 0x013AC708

void __fastcall SetCompStringColor();               // 0x00CB2D20

void __fastcall ImeUi_SetAppearance(const IMEUI_APPEARANCE* pia)
{
    if (!g_bInitialized || !pia)
        return;
    gSkinIME = *pia;
    gSkinIME.symbolColor &= 0xffffff;               // mask translucency
    gSkinIME.symbolColorOff &= 0xffffff;            // mask translucency
    gSkinIME.symbolColorText &= 0xffffff;           // mask translucency
    gSkinIME.compColorInput &= 0xffffff;            // mask translucency
    gSkinIME.compColorTargetConv &= 0xffffff;       // mask translucency
    gSkinIME.compColorConverted &= 0xffffff;        // mask translucency
    gSkinIME.compColorTargetNotConv &= 0xffffff;    // mask translucency
    gSkinIME.compColorInputErr &= 0xffffff;         // mask translucency
    SetCompStringColor();
}