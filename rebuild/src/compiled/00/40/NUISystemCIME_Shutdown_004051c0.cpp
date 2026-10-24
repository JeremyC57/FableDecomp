// NUISystem::CIME::Shutdown() -- tear the IME layer down when it was initialised: finalise and disable composition, re-initialise the window with the IME disabled, free the symbol font, then reset every member and the default appearance settings
// Leaf: ?Shutdown@CIME@NUISystem@@QAEXXZ
// Retail: Fable.exe @ 0x004051C0, 90 bytes, thiscall; called first thing by CIME::Initialise 0x00405650 (landed, which declares this
//   address as a "NativeCIME::Reset" adapter) and by the front end. Ego_r.pdb lists ?Shutdown@CIME@NUISystem@@QAEXXZ and a private
//   ?Reset@CIME@NUISystem@@AAEXXZ; this body is Shutdown with Reset inlined (Reset's stores + the ResetToDefaultSettings tail call).
//   The catalog label _Cons_val<...CScreenFilterEntry> is propagated.
// Callees: ImeUi_FinalizeString(bool) 0x00CB2C60, ImeUi_EnableIme(bool) 0x00CB5130, ImeUI_Initialize(HWND__*, bool) 0x00CB5180
//   (all __fastcall, as the landed Initialise declares them), operator delete 0x00BFE9BC, CIME::ResetToDefaultSettings() 0x00404A90
//   (static __fastcall, landed; reached by a tail jump).
// Layout: Ego_r.pdb CIME (0x54): EnableIME +0, WindowHandle +4, IsInitialised +8, Appearance +0xC (IMEUI_APPEARANCE, symbolFont at
//   +0x10 = this+0x1C), PEditBox +0x4C, CancelOnOKClick +0x50. rebuild/include/fable_ime.h has the same layout but keeps the members
//   private with no Shutdown/Reset declared, so the class is modelled locally with the PDB names.
// Shape: one ebx zero register for the flag test and every clear; CImeUiFont_Base has no virtual destructor, so `delete` is a bare
//   operator delete; memset of the 64-byte appearance is the inline `rep stosd`.
#include <string.h>
struct HWND__;
struct CEditBox;
class CImeUiFont_Base {
public:
    virtual void SetHeight(unsigned int) = 0;
    virtual void SetColor(unsigned long) = 0;
    virtual void SetPosition(int, int) = 0;
    virtual void GetTextExtent(const wchar_t*, unsigned long*, unsigned long*) = 0;
    virtual void DrawTextW(const wchar_t*) = 0;
};
struct IMEUI_APPEARANCE {                         // 0x40 bytes
    unsigned long    symbolColor, symbolColorOff, symbolColorText;
    unsigned char    symbolHeight, symbolTranslucence, symbolPlacement;
    CImeUiFont_Base* symbolFont;                  // +0x10
    unsigned long    candColorBase, candColorBorder, candColorText;
    unsigned long    compColorInput, compColorTargetConv, compColorConverted, compColorTargetNotConv, compColorInputErr;
    unsigned char    compTranslucence;
    unsigned long    compColorText;
    unsigned char    caretWidth, caretYMargin;
};
bool __fastcall ImeUI_Initialize(HWND__* window, bool disable);   // 0x00CB5180
void __fastcall ImeUi_EnableIme(bool enable);                    // 0x00CB5130
void __fastcall ImeUi_FinalizeString(bool send);                 // 0x00CB2C60
namespace NUISystem {
class CIME {
public:
    static void __fastcall ResetToDefaultSettings();             // 0x00404A90
    void Shutdown();
private:
    void Reset()
    {
        EnableIME = false;
        IsInitialised = false;
        WindowHandle = 0;
        PEditBox = 0;
        memset(&Appearance, 0, sizeof(Appearance));
        ResetToDefaultSettings();
    }
    bool             EnableIME;                   // +0x00
    HWND__*          WindowHandle;                // +0x04
    bool             IsInitialised;               // +0x08
    IMEUI_APPEARANCE Appearance;                  // +0x0C
    CEditBox*        PEditBox;                    // +0x4C
    bool             CancelOnOKClick;             // +0x50
};
void CIME::Shutdown()
{
    if (IsInitialised) {
        ImeUi_FinalizeString(false);
        ImeUi_EnableIme(false);
        ImeUI_Initialize(WindowHandle, true);
        IsInitialised = false;
        delete Appearance.symbolFont;
        Appearance.symbolFont = 0;
        Reset();
    }
}
}