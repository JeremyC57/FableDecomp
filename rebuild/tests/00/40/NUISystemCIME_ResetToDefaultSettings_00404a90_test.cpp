#include <cstdio>
class CImeUiFont_Base;
struct IMEUI_APPEARANCE {
    unsigned long    symbolColor;                        // +0x00
    unsigned long    symbolColorOff;                     // +0x04
    unsigned long    symbolColorText;                    // +0x08
    unsigned char    symbolHeight;                       // +0x0c
    unsigned char    symbolTranslucence;                 // +0x0d
    unsigned char    symbolPlacement;                    // +0x0e
    CImeUiFont_Base* symbolFont;                         // +0x10
    unsigned long    candColorBase;                      // +0x14
    unsigned long    candColorBorder;                    // +0x18
    unsigned long    candColorText;                      // +0x1c
    unsigned long    compColorInput;                     // +0x20
    unsigned long    compColorTargetConv;                // +0x24
    unsigned long    compColorConverted;                 // +0x28
    unsigned long    compColorTargetNotConv;             // +0x2c
    unsigned long    compColorInputErr;                  // +0x30
    unsigned char    compTranslucence;                   // +0x34
    unsigned long    compColorText;                      // +0x38
    unsigned char    caretWidth;                         // +0x3c
    unsigned char    caretYMargin;                       // +0x3d
};
namespace NUISystem {
class CIME {
public:
    static IMEUI_APPEARANCE DefaultAppearance;           // 0x013B7CE4 (name not in the PDB list)
    static void __fastcall ResetToDefaultSettings();
};

void __fastcall CIME::ResetToDefaultSettings()
{
    DefaultAppearance.symbolColor            = 0;
    DefaultAppearance.symbolColorOff         = 0x404040;
    DefaultAppearance.symbolColorText        = 0xff000000;
    DefaultAppearance.symbolHeight           = 24;
    DefaultAppearance.symbolTranslucence     = 0xa0;
    DefaultAppearance.symbolPlacement        = 0;
    DefaultAppearance.symbolFont             = 0;
    DefaultAppearance.candColorBase          = 0xffffffff;
    DefaultAppearance.candColorBorder        = 0xff000000;
    DefaultAppearance.candColorText          = 0;
    DefaultAppearance.compColorInput         = 0xffff00;
    DefaultAppearance.compColorTargetConv    = 0xff;
    DefaultAppearance.compColorConverted     = 0xff00;
    DefaultAppearance.compColorTargetNotConv = 0xff0000;
    DefaultAppearance.compColorInputErr      = 0xff0000;
    DefaultAppearance.compTranslucence       = 0x80;
    DefaultAppearance.compColorText          = 0;
    DefaultAppearance.caretWidth             = 2;
    DefaultAppearance.caretYMargin           = 1;
}
}

IMEUI_APPEARANCE NUISystem::CIME::DefaultAppearance;
int main()
{
    unsigned char* raw = (unsigned char*)&NUISystem::CIME::DefaultAppearance;
    for (unsigned i = 0; i < sizeof(IMEUI_APPEARANCE); ++i) raw[i] = 0x5a;
    NUISystem::CIME::ResetToDefaultSettings();
    const IMEUI_APPEARANCE& a = NUISystem::CIME::DefaultAppearance;
    bool ok = sizeof(IMEUI_APPEARANCE) == 0x40
        && a.symbolColor == 0 && a.symbolColorOff == 0x404040 && a.symbolColorText == 0xff000000
        && a.symbolHeight == 24 && a.symbolTranslucence == 0xa0 && a.symbolPlacement == 0 && a.symbolFont == 0
        && a.candColorBase == 0xffffffff && a.candColorBorder == 0xff000000 && a.candColorText == 0
        && a.compColorInput == 0xffff00 && a.compColorTargetConv == 0xff && a.compColorConverted == 0xff00
        && a.compColorTargetNotConv == 0xff0000 && a.compColorInputErr == 0xff0000 && a.compTranslucence == 0x80
        && a.compColorText == 0 && a.caretWidth == 2 && a.caretYMargin == 1
        && raw[0x0f] == 0x5a && raw[0x3e] == 0x5a;      // padding untouched
    std::printf(ok ? "00404a90_TEST PASS\n" : "00404a90_TEST FAIL\n");
    return ok ? 0 : 1;
}