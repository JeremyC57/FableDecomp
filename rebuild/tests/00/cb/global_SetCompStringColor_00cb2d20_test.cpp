// Behaviour test for 0x00CB2D20 ImeUi::SetCompStringColor: the five composition colours get alpha 0xff at IME level 2,
// else the skin's composition translucence.
#include <cstdio>

struct IMEUI_APPEARANCE_ {
    unsigned int compColorInput, compColorTargetConv, compColorConverted, compColorTargetNotConv, compColorInputErr;
    unsigned char compTranslucence;
};
struct _SkinCompStr { unsigned int colorInput, colorTargetConv, colorConverted, colorTargetNotConv, colorInputErr; };

static unsigned int g_dwIMELevel = 2;
static IMEUI_APPEARANCE_ gSkinIME = { 0x000001, 0x000202, 0x030303, 0x040000, 0x0000ff, 0x80 };
static _SkinCompStr gSkinCompStr;

static void SetCompStringColor()
{
    unsigned int dwTranslucency = (g_dwIMELevel == 2) ? 0xff000000 : ((unsigned int)gSkinIME.compTranslucence << 24);
    gSkinCompStr.colorInput = dwTranslucency | gSkinIME.compColorInput;
    gSkinCompStr.colorTargetConv = dwTranslucency | gSkinIME.compColorTargetConv;
    gSkinCompStr.colorConverted = dwTranslucency | gSkinIME.compColorConverted;
    gSkinCompStr.colorTargetNotConv = dwTranslucency | gSkinIME.compColorTargetNotConv;
    gSkinCompStr.colorInputErr = dwTranslucency | gSkinIME.compColorInputErr;
}

int main()
{
    SetCompStringColor();
    if (gSkinCompStr.colorInput != 0xff000001 || gSkinCompStr.colorTargetConv != 0xff000202 ||
        gSkinCompStr.colorConverted != 0xff030303 || gSkinCompStr.colorTargetNotConv != 0xff040000 ||
        gSkinCompStr.colorInputErr != 0xff0000ff) { std::printf("00cb2d20_TEST FAIL\n"); return 1; }
    g_dwIMELevel = 3;
    SetCompStringColor();
    if (gSkinCompStr.colorInput != 0x80000001 || gSkinCompStr.colorInputErr != 0x800000ff) { std::printf("00cb2d20_TEST FAIL\n"); return 1; }
    std::printf("00cb2d20_TEST PASS\n");
    return 0;
}