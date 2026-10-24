// LocalizedDialogBox(HINSTANCE hInstance, const char* templateName, HWND parent, DLGPROC dialogProc) -- run a modal dialog in the module language, falling back to US English (0x409) and then to DialogBoxParamA
// Retail: Fable.exe (TLC, VC7.1) @ 0x0099D520, 128 bytes. FableWin.pdb `@LocalizedDialogBox@16`: extern "C"
// `__fastcall` (hInstance in ecx, the template name in edx, parent and dialog procedure on the stack, `ret 8`).
// Callees: FindResourceExA @ [0x0143FD8C], LoadResource @ [0x0143FD90], LockResource @ [0x0143FD94] (KERNEL32),
// DialogBoxIndirectParamA @ [0x014402D8], DialogBoxParamA @ [0x014402DC] (USER32), LocalizedDialogBoxByLanguage
// @ 0x0099D4D0 (`__fastcall`, this chunk; its first call is expanded inline, the 0x409 retry is a real call).
// Globals: the module's UI language word @ 0x0138E060.
// Shape: the plain same-TU form -- VC7.1 itself expands the first LocalizedDialogBoxByLanguage call (module language)
// and keeps the English retry a call; each `!= -1` result returns through the shared epilogue.
// Leaf: @LocalizedDialogBox@16
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
extern "C" __declspec(dllimport) int __stdcall DialogBoxParamA(HINSTANCE instance, const char* templateName, HWND parent,
    DLGPROC dialogProc, long initParam);                                                                 // [0x014402DC]
extern "C" __declspec(dllimport) int __stdcall DialogBoxIndirectParamA(HINSTANCE instance, const DLGTEMPLATE* dialogTemplate,
    HWND parent, DLGPROC dialogProc, long initParam);                                                    // [0x014402D8]

#define RT_DIALOG_ID ((const char*)5)

extern unsigned short g_ModuleLanguage;   // 0x0138E060

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

extern "C" int __fastcall LocalizedDialogBox(HINSTANCE hInstance, const char* templateName, HWND parent, DLGPROC dialogProc)
{
    int result = LocalizedDialogBoxByLanguage(hInstance, templateName, parent, dialogProc, g_ModuleLanguage);
    if (result != -1)
        return result;
    if (g_ModuleLanguage != 0x409) {
        result = LocalizedDialogBoxByLanguage(hInstance, templateName, parent, dialogProc, 0x409);
        if (result != -1)
            return result;
    }
    return DialogBoxParamA(hInstance, templateName, parent, dialogProc, 0);
}