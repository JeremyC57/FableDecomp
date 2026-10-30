// DrawCaret(unsigned long x, unsigned long y, unsigned long height) -- DXUT ImeUi.cpp: when the caret blink is on, draw it as a caretWidth-wide rectangle through ImeUiCallback_DrawRect
// Shape (retry1): parked as an "LTCG custom convention" gap; ImeUi.cpp's file statics (FableWin.pdb local procs) are written as `static __declspec(noinline)` with a same-TU caller, and VC7.1 /O2 picks retail's private register convention itself. DrawCaret: x in ECX, y in ESI, height in EBX.
// Leaf: ?DrawCaret@@YAXKKK@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CB2820, 50 bytes (+ int3 padding). File-static in ImeUi.cpp
// (FableWin.pdb "DrawCaret"); callers 0x00CB3C4D / 0x00CB3CD7. The manifest label "Graphics_DrawRectConditional" is
// descriptive.
// Callee: the ImeUiCallback_DrawRect pointer 0x0143CD64 (no convention in this SDK drop: /Gr __fastcall, x1/y1 in
// ecx/edx).
// Shape: the 2004/2005 SDK body has no `&& ImeUiCallback_DrawRect` test; caretWidth/caretYMargin are read as one
// dword (`movzx dl/dh`).
class CImeUiFont_Base;
struct IMEUI_APPEARANCE {
    unsigned long    symbolColor, symbolColorOff, symbolColorText;
    unsigned char    symbolHeight, symbolTranslucence, symbolPlacement;
    CImeUiFont_Base* symbolFont;
    unsigned long    candColorBase, candColorBorder, candColorText;
    unsigned long    compColorInput, compColorTargetConv, compColorConverted, compColorTargetNotConv, compColorInputErr;
    unsigned char    compTranslucence;
    unsigned long    compColorText;
    unsigned char    caretWidth;                    // +0x3c
    unsigned char    caretYMargin;                  // +0x3d
};
struct tagRECT { long left, top, right, bottom; };
struct CompStringAttribute {
    unsigned int     caretX, caretY;
    CImeUiFont_Base* pFont;
    unsigned long    colorComp;                     // +0x0c
    unsigned long    colorCand;
    tagRECT          margins;
};

extern bool                g_bCaretDraw;            // 0x0143E8AC
extern IMEUI_APPEARANCE    gSkinIME;                // 0x013AC708
extern CompStringAttribute g_CaretInfo;             // 0x0143CD2C
extern void (__fastcall* ImeUiCallback_DrawRect)(int x1, int y1, int x2, int y2, unsigned long color);   // 0x0143CD64

static __declspec(noinline) void DrawCaret(unsigned long x, unsigned long y, unsigned long height)
{
    if (g_bCaretDraw)
        ImeUiCallback_DrawRect(x, y + gSkinIME.caretYMargin, x + gSkinIME.caretWidth,
                               y + height - gSkinIME.caretYMargin, g_CaretInfo.colorComp);
}

// Same-TU caller (the DrawCaret calls of ImeUi.cpp DrawCompositionString), present so the static gets retail's private convention. Not graded.
void ImeUi_DrawCaret_caller(unsigned long x, unsigned long y, unsigned long height)
{
    DrawCaret(x, y, height);
}