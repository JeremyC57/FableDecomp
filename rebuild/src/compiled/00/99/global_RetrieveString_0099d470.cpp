// RetrieveString(HINSTANCE hInstance, unsigned int id, char* buffer, int bufferSize) -- load a string-table entry in the module language, falling back to US English (0x409) and then to LoadStringA
// Retail: Fable.exe (TLC, VC7.1) @ 0x0099D470, 84 bytes. FableWin.pdb `@RetrieveString@16`: extern "C" `__fastcall`
// (hInstance in ecx, id in edx, buffer and size on the stack, `ret 8`).
// Callees: RetrieveStringByLanguage @ 0x0099D3A0 (`__fastcall`, this chunk), LoadStringA @ [0x014402D4] (USER32).
// Globals: the module's UI language word @ 0x0138E060 (zero-extended into the pushed WORD argument; compared with
// 0x409 straight from memory).
// Label correction: the manifest's "Utility_LoadStringByLanguageWithFallback" is a descriptive stand-in; the PDB name
// is RetrieveString.
// Shape: two early `return result` exits merge on the shared epilogue; the English retry is skipped when the
// module language already is 0x409.
// Leaf: @RetrieveString@16
struct HINSTANCE__ { int unused; };
typedef HINSTANCE__* HINSTANCE;

extern "C" __declspec(dllimport) int __stdcall LoadStringA(HINSTANCE instance, unsigned int id, char* buffer,
                                                           int bufferSize);                  // [0x014402D4]
int __fastcall RetrieveStringByLanguage(HINSTANCE hInstance, unsigned long id, char* buffer,
                                        unsigned long bufferSize, unsigned short language);  // 0x0099D3A0

extern unsigned short g_ModuleLanguage;   // 0x0138E060

extern "C" int __fastcall RetrieveString(HINSTANCE hInstance, unsigned int id, char* buffer, int bufferSize)
{
    int result = RetrieveStringByLanguage(hInstance, id, buffer, bufferSize, g_ModuleLanguage);
    if (result != 0)
        return result;
    if (g_ModuleLanguage != 0x409) {
        result = RetrieveStringByLanguage(hInstance, id, buffer, bufferSize, 0x409);
        if (result != 0)
            return result;
    }
    return LoadStringA(hInstance, id, buffer, bufferSize);
}