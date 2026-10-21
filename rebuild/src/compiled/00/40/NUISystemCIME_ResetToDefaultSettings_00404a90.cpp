// NUISystem::CIME::ResetToDefaultSettings() -- reset the IME UI appearance (colours, sizes, caret) to the built-in defaults
// Leaf: ?ResetToDefaultSettings@CIME@NUISystem@@SIXXZ
// Retail: Fable.exe @ 0x00404A90, 147 bytes, plain `ret`, no `this`. Catalog label `$E2` is wrong: the callers are the
// CIME constructor (0x00404F20 / 0x00405A30 zero EnableIME, WindowHandle, IsInitialised, the 16-dword Appearance and
// PEditBox, then tail-call / call this) and the global CIME's dynamic initializer 0x01219E90 (calls this with no ecx, then
// atexit). Ego_r.pdb names the static, no-argument CIME member ?ResetToDefaultSettings@CIME@NUISystem@@SIXXZ (`SI`: static
// __fastcall); CIME::LoadIMESettings (the "SymbolColor" / "CandColorBase" / ... / "CaretYMargin" readers at 0x004053F8 on)
// writes the same block.
// Body: writes the 0x40-byte IMEUI_APPEARANCE at 0x013B7CE4 member by member: the DirectX SDK ImeUi defaults
// (symbolColorOff 0x404040, symbolColorText 0xff000000, symbolHeight 24, symbolTranslucence 0xa0, candColorBase
// 0xffffffff, candColorBorder 0xff000000, compColorInput 0xffff00, ..., compTranslucence 0x80, caretWidth 2,
// caretYMargin 1), symbolFont NULL.
// Layout: Ego_r.pdb IMEUI_APPEARANCE (0x40, 19 members, symbolFont +0x10). The block's own symbol is not in the PDB name
// list; it is modelled as a static CIME member (CIME::Appearance at +0xc is the per-instance copy).
// Shape: /O2 (16-aligned entry).
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