// ImeUi_SetCompStringAppearance(CImeUiFont_Base* pFont, unsigned long color, const RECT* prc) -- DXUT ImeUi.cpp: remember the composition-string font and margins, and take the candidate/composition text colours from the skin (or `color` when the skin leaves them 0)
// Leaf: ?ImeUi_SetCompStringAppearance@@YIXPAVCImeUiFont_Base@@KPBUtagRECT@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CB2880, 96 bytes. __fastcall (pFont ecx, color edx, prc on the
// stack, `ret 4`); the FableWin.pdb name is ?ImeUi_SetCompStringAppearance@@YIXPAVCImeUiFont_Base@@KPBUtagRECT@@@Z.
// The manifest label is right.
// Globals: g_bInitialized 0x0143E8AE, g_CaretInfo 0x0143CD2C (pFont +8, colorComp +0x0c, colorCand +0x10,
// margins +0x14), gSkinIME 0x013AC708 (candColorText +0x1c, compColorText +0x38).
// Shape: the DXUT body verbatim; each `if (0 == skin) colour = color; else colour = skin;` becomes a store of color
// followed by a conditional store of the skin value.
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

struct CompStringAttribute {                        // ImeUi.cpp
    unsigned int     caretX;                        // +0x00
    unsigned int     caretY;                        // +0x04
    CImeUiFont_Base* pFont;                         // +0x08
    unsigned long    colorComp;                     // +0x0c
    unsigned long    colorCand;                     // +0x10
    RECT             margins;                       // +0x14
};

extern bool                g_bInitialized;          // 0x0143E8AE
extern CompStringAttribute g_CaretInfo;             // 0x0143CD2C
extern IMEUI_APPEARANCE    gSkinIME;                // 0x013AC708

void __fastcall ImeUi_SetCompStringAppearance(CImeUiFont_Base* pFont, unsigned long color, const RECT* prc)
{
    if (!g_bInitialized)
        return;
    g_CaretInfo.pFont = pFont;
    g_CaretInfo.margins = *prc;

    if (0 == gSkinIME.candColorText)
        g_CaretInfo.colorCand = color;
    else
        g_CaretInfo.colorCand = gSkinIME.candColorText;
    if (0 == gSkinIME.compColorText)
        g_CaretInfo.colorComp = color;
    else
        g_CaretInfo.colorComp = gSkinIME.compColorText;
}