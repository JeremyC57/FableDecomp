#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __fastcall
#endif
// Test copy of the 0x004051C0 model: CIME::Shutdown with the private Reset inlined.
struct HWND__ { int unused; };
struct CEditBox { int unused; };
class CImeUiFont_Base {
public:
    virtual void SetHeight(unsigned int) = 0;
};
class CTestFont : public CImeUiFont_Base {
public:
    virtual void SetHeight(unsigned int) { }
};
struct IMEUI_APPEARANCE {
    unsigned long    symbolColor, symbolColorOff, symbolColorText;
    unsigned char    symbolHeight, symbolTranslucence, symbolPlacement;
    CImeUiFont_Base* symbolFont;
    unsigned long    candColorBase, candColorBorder, candColorText;
    unsigned long    compColorInput, compColorTargetConv, compColorConverted, compColorTargetNotConv, compColorInputErr;
    unsigned char    compTranslucence;
    unsigned long    compColorText;
    unsigned char    caretWidth, caretYMargin;
};
static int g_calls = 0, g_finalise = -1, g_enable = -1, g_initDisable = -1, g_defaults = 0;
static HWND__* g_initWindow = 0;
bool __fastcall ImeUI_Initialize(HWND__* window, bool disable) { ++g_calls; g_initWindow = window; g_initDisable = disable; return true; }
void __fastcall ImeUi_EnableIme(bool enable) { ++g_calls; g_enable = enable; }
void __fastcall ImeUi_FinalizeString(bool send) { ++g_calls; g_finalise = send; }
namespace NUISystem {
class CIME {
public:
    static void __fastcall ResetToDefaultSettings() { ++g_defaults; }
    void Shutdown();
    void TestSetup(HWND__* w, CEditBox* box, CImeUiFont_Base* font)
    {
        EnableIME = true; WindowHandle = w; IsInitialised = true; PEditBox = box; CancelOnOKClick = true;
        std::memset(&Appearance, 0x5A, sizeof(Appearance));
        Appearance.symbolFont = font;
    }
    bool TestIsReset() const
    {
        const unsigned char* bytes = reinterpret_cast<const unsigned char*>(&Appearance);
        for (unsigned int i = 0; i < sizeof(Appearance); ++i) if (bytes[i] != 0) return false;
        return !EnableIME && WindowHandle == 0 && !IsInitialised && PEditBox == 0 && CancelOnOKClick;
    }
    void TestSetInitialised(bool b) { IsInitialised = b; }
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
    bool             EnableIME;
    HWND__*          WindowHandle;
    bool             IsInitialised;
    IMEUI_APPEARANCE Appearance;
    CEditBox*        PEditBox;
    bool             CancelOnOKClick;
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
int main()
{
    HWND__ window;
    CEditBox box;
    NUISystem::CIME ime;
    ime.TestSetup(&window, &box, new CTestFont);
    ime.TestSetInitialised(false);
    ime.Shutdown();                              // not initialised: nothing happens
    bool ok = g_calls == 0 && g_defaults == 0;
    ime.TestSetInitialised(true);
    ime.Shutdown();
    if (g_calls != 3 || g_finalise != 0 || g_enable != 0 || g_initDisable != 1 || g_initWindow != &window) ok = false;
    if (g_defaults != 1 || !ime.TestIsReset()) ok = false;   // font freed and pointer cleared with the whole appearance
    if (!ok) { std::printf("004051c0_TEST FAIL\n"); return 1; }
    std::printf("004051c0_TEST PASS\n");
    return 0;
}