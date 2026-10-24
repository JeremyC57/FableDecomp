// LocalizedDialogBoxByLanguage(HINSTANCE hInstance, const char* templateName, HWND parent, DLGPROC dialogProc, unsigned short language) -- run a modal dialog from the template of one language
// Retail: Fable.exe (TLC, VC7.1) @ 0x0099D4D0, 76 bytes. FableWin.pdb
// ?LocalizedDialogBoxByLanguage@@YIHPAUHINSTANCE__@@PBDPAUHWND__@@P6GH2IIJ@ZG@Z: `__fastcall` (hInstance in ecx, the
// template name in edx, the rest on the stack, `ret 0Ch`).
// Callees: FindResourceExA @ [0x0143FD8C], LoadResource @ [0x0143FD90], LockResource @ [0x0143FD94] (KERNEL32),
// DialogBoxIndirectParamA @ [0x014402D8] (USER32).
// Shape: the RT_DIALOG (5) resource of the requested language is found, loaded and locked; each failure is its own
// early `return -1` (the merged block stays on the fall-through; nested ifs put it after the success arm)
// (`or eax,-1`); the success arm runs the dialog from the locked template with a zero init parameter.
// Leaf: ?LocalizedDialogBoxByLanguage@@
struct HINSTANCE__ { int unused; };
typedef HINSTANCE__* HINSTANCE;
typedef HINSTANCE HMODULE;
struct HWND__ { int unused; };
typedef HWND__* HWND;
struct HRSRC__ { int unused; };
typedef HRSRC__* HRSRC;
typedef void* HGLOBAL;
typedef int (__stdcall* DLGPROC)(HWND window, unsigned int message, unsigned int wParam, long lParam);
struct DLGTEMPLATE;

extern "C" __declspec(dllimport) HRSRC __stdcall FindResourceExA(HMODULE module, const char* type, const char* name,
                                                                 unsigned short language);   // [0x0143FD8C]
extern "C" __declspec(dllimport) HGLOBAL __stdcall LoadResource(HMODULE module, HRSRC resource);        // [0x0143FD90]
extern "C" __declspec(dllimport) void* __stdcall LockResource(HGLOBAL data);                            // [0x0143FD94]
extern "C" __declspec(dllimport) int __stdcall DialogBoxIndirectParamA(HINSTANCE instance, const DLGTEMPLATE* dialogTemplate,
    HWND parent, DLGPROC dialogProc, long initParam);                                                    // [0x014402D8]

#define RT_DIALOG_ID ((const char*)5)

int __fastcall LocalizedDialogBoxByLanguage(HINSTANCE hInstance, const char* templateName, HWND parent,
                                            DLGPROC dialogProc, unsigned short language)
{
    HRSRC resource = FindResourceExA(hInstance, RT_DIALOG_ID, templateName, language);
    if (resource == 0)
        return -1;
    HGLOBAL data = LoadResource(hInstance, resource);
    if (data == 0)
        return -1;
    const DLGTEMPLATE* dialogTemplate = (const DLGTEMPLATE*)LockResource(data);
    if (dialogTemplate == 0)
        return -1;
    return DialogBoxIndirectParamA(hInstance, dialogTemplate, parent, dialogProc, 0);
}