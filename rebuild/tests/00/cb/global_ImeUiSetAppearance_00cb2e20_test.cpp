// Behaviour test for 0x00CB2E20 ImeUi::ImeUi_SetAppearance: copy the 0x40-byte skin, strip the alpha byte of the three
// symbol and five composition colours (other fields untouched), then recompute the composition colours.
#include <cstdio>

class CImeUiFont_Base;
struct IMEUI_APPEARANCE {
    unsigned int     symbolColor, symbolColorOff, symbolColorText;
    unsigned char    symbolHeight, symbolTranslucence, symbolPlacement;
    CImeUiFont_Base* symbolFont;
    unsigned int     candColorBase, candColorBorder, candColorText;
    unsigned int     compColorInput, compColorTargetConv, compColorConverted, compColorTargetNotConv, compColorInputErr;
    unsigned char    compTranslucence;
    unsigned int     compColorText;
    unsigned char    caretWidth, caretYMargin;
};

static bool g_bInitialized = false;
static IMEUI_APPEARANCE gSkinIME;
static int g_colorCalls = 0;
static void SetCompStringColor() { ++g_colorCalls; }

static void ImeUi_SetAppearance(const IMEUI_APPEARANCE* pia)
{
    if (!g_bInitialized || !pia)
        return;
    gSkinIME = *pia;
    gSkinIME.symbolColor &= 0xffffff;
    gSkinIME.symbolColorOff &= 0xffffff;
    gSkinIME.symbolColorText &= 0xffffff;
    gSkinIME.compColorInput &= 0xffffff;
    gSkinIME.compColorTargetConv &= 0xffffff;
    gSkinIME.compColorConverted &= 0xffffff;
    gSkinIME.compColorTargetNotConv &= 0xffffff;
    gSkinIME.compColorInputErr &= 0xffffff;
    SetCompStringColor();
}

int main()
{
    IMEUI_APPEARANCE ia;
    ia.symbolColor = 0x11223344; ia.symbolColorOff = 0xff000001; ia.symbolColorText = 0x80abcdef;
    ia.symbolHeight = 24; ia.symbolTranslucence = 0x40; ia.symbolPlacement = 1; ia.symbolFont = 0;
    ia.candColorBase = 0xaa000001; ia.candColorBorder = 0xbb000002; ia.candColorText = 0xcc000003;
    ia.compColorInput = 0xff010101; ia.compColorTargetConv = 0xff020202; ia.compColorConverted = 0xff030303;
    ia.compColorTargetNotConv = 0xff040404; ia.compColorInputErr = 0xff050505;
    ia.compTranslucence = 0x99; ia.compColorText = 0xdd000004; ia.caretWidth = 2; ia.caretYMargin = 1;
    ImeUi_SetAppearance(&ia);
    if (g_colorCalls != 0 || gSkinIME.symbolColor != 0) { std::printf("00cb2e20_TEST FAIL\n"); return 1; }
    g_bInitialized = true;
    ImeUi_SetAppearance(0);
    if (g_colorCalls != 0) { std::printf("00cb2e20_TEST FAIL\n"); return 1; }
    ImeUi_SetAppearance(&ia);
    if (g_colorCalls != 1 || gSkinIME.symbolColor != 0x223344 || gSkinIME.symbolColorOff != 0x000001 ||
        gSkinIME.symbolColorText != 0xabcdef || gSkinIME.compColorInput != 0x010101 ||
        gSkinIME.compColorInputErr != 0x050505 || gSkinIME.candColorBase != 0xaa000001 ||
        gSkinIME.compColorText != 0xdd000004 || gSkinIME.symbolHeight != 24 || gSkinIME.caretWidth != 2 ||
        gSkinIME.compTranslucence != 0x99) { std::printf("00cb2e20_TEST FAIL\n"); return 1; }
    std::printf("00cb2e20_TEST PASS\n");
    return 0;
}