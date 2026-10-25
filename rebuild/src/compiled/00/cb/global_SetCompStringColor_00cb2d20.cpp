// SetCompStringColor() -- DXUT ImeUi.cpp: rebuild the five composition-string colours from the skin, opaque at IME level 2 and with the skin's composition translucence otherwise
// Leaf: ?SetCompStringColor@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CB2D20, 97 bytes (+ int3 padding). No arguments (file-static in
// ImeUi.cpp, FableWin.pdb "SetCompStringColor"; external here so the TU emits it). The manifest label is right.
// Globals: g_dwIMELevel 0x0143CD1C, gSkinIME 0x013AC708 (compColorInput.. +0x20..+0x30, compTranslucence +0x34),
// gSkinCompStr 0x0143CD08.
// Shape: the DXUT body verbatim (`(DWORD)gSkinIME.compTranslucence << 24` is `movzx ; shl 18h`).
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

struct _SkinCompStr {                               // ImeUi.cpp
    unsigned long colorInput;                       // +0x00
    unsigned long colorTargetConv;                  // +0x04
    unsigned long colorConverted;                   // +0x08
    unsigned long colorTargetNotConv;               // +0x0c
    unsigned long colorInputErr;                    // +0x10
};

extern unsigned long    g_dwIMELevel;               // 0x0143CD1C
extern IMEUI_APPEARANCE gSkinIME;                   // 0x013AC708
extern _SkinCompStr     gSkinCompStr;               // 0x0143CD08

void __fastcall SetCompStringColor()
{
    // change color setting according to current IME level.
    unsigned long dwTranslucency = (g_dwIMELevel == 2) ? 0xff000000 : ((unsigned long)gSkinIME.compTranslucence << 24);
    gSkinCompStr.colorInput = dwTranslucency | gSkinIME.compColorInput;
    gSkinCompStr.colorTargetConv = dwTranslucency | gSkinIME.compColorTargetConv;
    gSkinCompStr.colorConverted = dwTranslucency | gSkinIME.compColorConverted;
    gSkinCompStr.colorTargetNotConv = dwTranslucency | gSkinIME.compColorTargetNotConv;
    gSkinCompStr.colorInputErr = dwTranslucency | gSkinIME.compColorInputErr;
}