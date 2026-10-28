// _HyperlinkProc(HWND window, unsigned int message, unsigned int wParam, long lParam) -- window procedure of a static control converted to a hyperlink: unsubclass and drop its props on WM_DESTROY, track the hover (capture + repaint) on WM_MOUSEMOVE, show the hand cursor on WM_SETCURSOR, else forward to the original procedure
// Leaf: ?_HyperlinkProc@@YGJPAUHWND__@@IIJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0099D660, 363 bytes, `ret 10h` (__stdcall). FableWin.pdb ?_HyperlinkProc@@YGJPAUHWND__@@IIJ@Z;
//   installed by the landed ConvertStaticToHyperlink 0x0099D7D0 (the only reference, 0x0099D843).
// Callees (USER32): GetPropA [0x014402EC], GetCapture [0x01440314], InvalidateRect [0x01440310], SetCapture [0x0144030C],
//   GetWindowRect [0x01440308], ClientToScreen [0x01440304], PtInRect [0x01440300], ReleaseCapture [0x014402FC],
//   LoadCursorA [0x014402F8], SetCursor [0x014402F4], SetWindowLongA [0x014402E4], RemovePropA [0x014402E0],
//   SendMessageA [0x014402F0], CallWindowProcA [0x014402E8]; (GDI32): DeleteObject [0x0143FC74].
// Data: g_HyperlinkHovered @ 0x013BD6F8 (descriptive; a BOOL also read by _HyperlinkParentProc 0x0099D5B0 to colour
//   the text); strings "Old_Proc" 0x0129A974, "Old_Font" 0x0129A988, "Font" 0x0129A980, "Static" 0x0129A96C.
// Shape: Neal Stublen's CodeProject hyperlink procedure, with Fable's hover flag in place of the underline-font swap;
//   GetPropA and RemovePropA are cached in callee-saved registers by VC7.1.
struct HWND__ { int unused; };
typedef HWND__* HWND;
typedef void* HANDLE;
typedef void* HGDIOBJ;
struct HICON__ { int unused; };
typedef HICON__* HCURSOR;
struct HINSTANCE__ { int unused; };
typedef HINSTANCE__* HINSTANCE;
struct RECT { long left, top, right, bottom; };
struct POINT { long x, y; };
typedef long (__stdcall* WNDPROC)(HWND window, unsigned int message, unsigned int wParam, long lParam);

extern "C" __declspec(dllimport) HANDLE __stdcall GetPropA(HWND window, const char* name);
extern "C" __declspec(dllimport) HANDLE __stdcall RemovePropA(HWND window, const char* name);
extern "C" __declspec(dllimport) HWND __stdcall GetCapture();
extern "C" __declspec(dllimport) HWND __stdcall SetCapture(HWND window);
extern "C" __declspec(dllimport) int __stdcall ReleaseCapture();
extern "C" __declspec(dllimport) int __stdcall InvalidateRect(HWND window, const RECT* rect, int erase);
extern "C" __declspec(dllimport) int __stdcall GetWindowRect(HWND window, RECT* rect);
extern "C" __declspec(dllimport) int __stdcall ClientToScreen(HWND window, POINT* point);
extern "C" __declspec(dllimport) int __stdcall PtInRect(const RECT* rect, POINT point);
extern "C" __declspec(dllimport) HCURSOR __stdcall LoadCursorA(HINSTANCE instance, const char* name);
extern "C" __declspec(dllimport) HCURSOR __stdcall SetCursor(HCURSOR cursor);
extern "C" __declspec(dllimport) long __stdcall SetWindowLongA(HWND window, int index, long value);
extern "C" __declspec(dllimport) long __stdcall SendMessageA(HWND window, unsigned int message, unsigned int wParam, long lParam);
extern "C" __declspec(dllimport) long __stdcall CallWindowProcA(WNDPROC proc, HWND window, unsigned int message,
                                                                unsigned int wParam, long lParam);
extern "C" __declspec(dllimport) int __stdcall DeleteObject(HGDIOBJ object);

extern int g_HyperlinkHovered;                          // 0x013BD6F8 (descriptive)

#define PROP_ORIGINAL_PROC      "Old_Proc"
#define PROP_ORIGINAL_FONT      "Old_Font"
#define PROP_UNDERLINE_FONT     "Font"
#define PROP_STATIC_HYPERLINK   "Static"

long __stdcall _HyperlinkProc(HWND window, unsigned int message, unsigned int wParam, long lParam)
{
    WNDPROC original_proc = (WNDPROC)GetPropA(window, PROP_ORIGINAL_PROC);

    switch (message) {
    case 0x0002: {                                      // WM_DESTROY
        SetWindowLongA(window, -4, (long)original_proc);    // GWL_WNDPROC
        RemovePropA(window, PROP_ORIGINAL_PROC);

        HGDIOBJ original_font = GetPropA(window, PROP_ORIGINAL_FONT);
        SendMessageA(window, 0x0030, (unsigned int)original_font, 0);   // WM_SETFONT
        RemovePropA(window, PROP_ORIGINAL_FONT);

        HGDIOBJ font = GetPropA(window, PROP_UNDERLINE_FONT);
        DeleteObject(font);
        RemovePropA(window, PROP_UNDERLINE_FONT);

        RemovePropA(window, PROP_STATIC_HYPERLINK);
        break;
    }
    case 0x0200: {                                      // WM_MOUSEMOVE
        if (GetCapture() != window) {
            g_HyperlinkHovered = 1;
            InvalidateRect(window, 0, 0);
            SetCapture(window);
        } else {
            RECT rect;
            GetWindowRect(window, &rect);

            POINT point = { (unsigned short)lParam, (unsigned short)((unsigned long)lParam >> 16) };
            ClientToScreen(window, &point);

            if (!PtInRect(&rect, point)) {
                g_HyperlinkHovered = 0;
                InvalidateRect(window, 0, 0);
                ReleaseCapture();
            }
        }
        break;
    }
    case 0x0020: {                                      // WM_SETCURSOR
        HCURSOR cursor = LoadCursorA(0, (const char*)32649);   // IDC_HAND
        if (cursor == 0)
            cursor = LoadCursorA(0, (const char*)32512);       // IDC_ARROW
        SetCursor(cursor);
        return 1;
    }
    }

    return CallWindowProcA(original_proc, window, message, wParam, lParam);
}