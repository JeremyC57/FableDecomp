// ConvertStaticToHyperlink(HWND control) -- subclass a static control (and its parent) so it draws and behaves as an underlined hyperlink
// Retail: Fable.exe (TLC, VC7.1) @ 0x0099D7D0, 231 bytes, /GS (cookie @ 0x0139C8A8, __security_check_cookie
// @ 0x00BFE9F9). FableWin.pdb ?ConvertStaticToHyperlink@@YIHPAUHWND__@@@Z: `__fastcall` (control in ecx), returns TRUE.
// Callees (USER32): GetParent [0x01440320], GetWindowLongA [0x01440318], SetWindowLongA [0x014402E4], SetPropA
// [0x0144031C], SendMessageA [0x014402F0]; (GDI32): GetObjectA [0x0143FC6C], CreateFontIndirectA [0x0143FC70].
// Window procedures: _HyperlinkParentProc @ 0x0099D5B0, _HyperlinkProc @ 0x0099D660 (both __stdcall, same TU).
// Strings: "Old_Proc" 0x0129A974, "Old_Font" 0x0129A988, "Font" 0x0129A980, "Static" 0x0129A96C.
// Shape: Neal Stublen's CodeProject "static control to hyperlink" routine: subclass the parent once (skipped when its
// window procedure already is _HyperlinkParentProc), add SS_NOTIFY (0x100), subclass the control, keep its font as
// "Old_Font", build an underlined copy from the LOGFONTA (0x3C bytes, the /GS buffer) and select it with WM_SETFONT,
// then tag the control "Static" = 1. The repeated imports are cached in callee-saved registers by VC7.1.
// Flags: /GS
// Leaf: ?ConvertStaticToHyperlink@@
struct HWND__ { int unused; };
typedef HWND__* HWND;
typedef void* HANDLE;
typedef void* HGDIOBJ;
struct HFONT__ { int unused; };
typedef HFONT__* HFONT;
typedef long (__stdcall* WNDPROC)(HWND window, unsigned int message, unsigned int wParam, long lParam);

struct LOGFONTA {                    // 60 bytes
    long          lfHeight;          // +0x00
    long          lfWidth;           // +0x04
    long          lfEscapement;      // +0x08
    long          lfOrientation;     // +0x0C
    long          lfWeight;          // +0x10
    unsigned char lfItalic;          // +0x14
    unsigned char lfUnderline;       // +0x15
    unsigned char lfStrikeOut;       // +0x16
    unsigned char lfCharSet;         // +0x17
    unsigned char lfOutPrecision;    // +0x18
    unsigned char lfClipPrecision;   // +0x19
    unsigned char lfQuality;         // +0x1A
    unsigned char lfPitchAndFamily;  // +0x1B
    char          lfFaceName[32];    // +0x1C
};

extern "C" __declspec(dllimport) HWND __stdcall GetParent(HWND window);                                  // [0x01440320]
extern "C" __declspec(dllimport) long __stdcall GetWindowLongA(HWND window, int index);                  // [0x01440318]
extern "C" __declspec(dllimport) long __stdcall SetWindowLongA(HWND window, int index, long value);      // [0x014402E4]
extern "C" __declspec(dllimport) int __stdcall SetPropA(HWND window, const char* name, HANDLE data);     // [0x0144031C]
extern "C" __declspec(dllimport) long __stdcall SendMessageA(HWND window, unsigned int message, unsigned int wParam,
                                                             long lParam);                               // [0x014402F0]
extern "C" __declspec(dllimport) int __stdcall GetObjectA(HGDIOBJ object, int size, void* buffer);      // [0x0143FC6C]
extern "C" __declspec(dllimport) HFONT __stdcall CreateFontIndirectA(const LOGFONTA* font);             // [0x0143FC70]

long __stdcall _HyperlinkParentProc(HWND window, unsigned int message, unsigned int wParam, long lParam);   // 0x0099D5B0
long __stdcall _HyperlinkProc(HWND window, unsigned int message, unsigned int wParam, long lParam);         // 0x0099D660

#define GWL_WNDPROC_ID  (-4)
#define GWL_STYLE_ID    (-16)
#define SS_NOTIFY_BIT   0x100
#define WM_SETFONT_ID   0x30
#define WM_GETFONT_ID   0x31

int __fastcall ConvertStaticToHyperlink(HWND control)
{
    // Subclass the parent so the control can be coloured.
    HWND parent = GetParent(control);
    if (parent != 0) {
        WNDPROC parentProc = (WNDPROC)GetWindowLongA(parent, GWL_WNDPROC_ID);
        if (parentProc != _HyperlinkParentProc) {
            SetPropA(parent, "Old_Proc", (HANDLE)parentProc);
            SetWindowLongA(parent, GWL_WNDPROC_ID, (long)(WNDPROC)_HyperlinkParentProc);
        }
    }

    // Make sure the control sends notifications.
    unsigned long style = GetWindowLongA(control, GWL_STYLE_ID);
    SetWindowLongA(control, GWL_STYLE_ID, style | SS_NOTIFY_BIT);

    // Subclass the control.
    WNDPROC controlProc = (WNDPROC)GetWindowLongA(control, GWL_WNDPROC_ID);
    SetPropA(control, "Old_Proc", (HANDLE)controlProc);
    SetWindowLongA(control, GWL_WNDPROC_ID, (long)(WNDPROC)_HyperlinkProc);

    // Underlined copy of the control's font.
    HFONT originalFont = (HFONT)SendMessageA(control, WM_GETFONT_ID, 0, 0);
    SetPropA(control, "Old_Font", (HANDLE)originalFont);

    LOGFONTA logFont;
    GetObjectA(originalFont, sizeof(logFont), &logFont);
    logFont.lfUnderline = 1;

    HFONT underlineFont = CreateFontIndirectA(&logFont);
    SetPropA(control, "Font", (HANDLE)underlineFont);
    SendMessageA(control, WM_SETFONT_ID, (unsigned int)underlineFont, 0);

    // Tag the control so the parent knows how to colour it.
    SetPropA(control, "Static", (HANDLE)1);

    return 1;
}