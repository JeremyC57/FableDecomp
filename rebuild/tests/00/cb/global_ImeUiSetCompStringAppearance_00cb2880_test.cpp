// Behaviour test for 0x00CB2880 ImeUi::ImeUi_SetCompStringAppearance: nothing before init; else font + margins stored,
// candidate/composition colours from the skin or `color` when the skin colour is 0.
#include <cstdio>

struct RECT_ { int left, top, right, bottom; };
class CImeUiFont_Base;
struct IMEUI_APPEARANCE_ { unsigned int candColorText, compColorText; };
struct CompStringAttribute { unsigned int caretX, caretY; CImeUiFont_Base* pFont; unsigned int colorComp, colorCand; RECT_ margins; };

static bool g_bInitialized = false;
static CompStringAttribute g_CaretInfo;
static IMEUI_APPEARANCE_ gSkinIME;

static void ImeUi_SetCompStringAppearance(CImeUiFont_Base* pFont, unsigned int color, const RECT_* prc)
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

int main()
{
    CImeUiFont_Base* font = (CImeUiFont_Base*)&g_CaretInfo;
    RECT_ rc = { 1, 2, 3, 4 };
    ImeUi_SetCompStringAppearance(font, 0x123456, &rc);
    if (g_CaretInfo.pFont != 0 || g_CaretInfo.colorCand != 0) { std::printf("00cb2880_TEST FAIL\n"); return 1; }
    g_bInitialized = true;
    gSkinIME.candColorText = 0;
    gSkinIME.compColorText = 0xff00ff;
    ImeUi_SetCompStringAppearance(font, 0x123456, &rc);
    if (g_CaretInfo.pFont != font || g_CaretInfo.margins.left != 1 || g_CaretInfo.margins.bottom != 4 ||
        g_CaretInfo.colorCand != 0x123456 || g_CaretInfo.colorComp != 0xff00ff) {
        std::printf("00cb2880_TEST FAIL\n");
        return 1;
    }
    gSkinIME.candColorText = 0xabcdef;
    gSkinIME.compColorText = 0;
    ImeUi_SetCompStringAppearance(font, 0x777, &rc);
    if (g_CaretInfo.colorCand != 0xabcdef || g_CaretInfo.colorComp != 0x777) { std::printf("00cb2880_TEST FAIL\n"); return 1; }
    std::printf("00cb2880_TEST PASS\n");
    return 0;
}