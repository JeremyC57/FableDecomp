// EULA::DoEULAThings() -- load eula.dll, run its EBUEula export with the registry-key location and the EULA / warranty file names; 1 when accepted (the library is then freed), else 0
// Leaf: DoEULAThings
// Retail: Fable.exe @ 0x00401FE0, 136 bytes, `__fastcall` with no arguments, plain `ret` (FableWin.pdb ?DoEULAThings@EULA@@YIHXZ:
// a namespace function returning int). Replaces the landed asm_bake oracle rebuild/src/compiled/00/40/EULA_DoEULAThings_00401fe0.cpp.
// Callees: imports LoadLibraryW 0x0143FE38, GetProcAddress 0x0143FE44, FreeLibrary 0x0143FE3C; CWideString::ConvertToCharString
// 0x0099B2C0 (by-value CCharString through a hidden pointer) on the global 0x013BCA28, ~CCharString 0x0099EAE0; the export
// is `__stdcall` (it pops its four arguments). Data: L"eula.dll" 0x0122D680, "EBUEula" 0x0122D678, "" 0x0129AAF4, the two
// file-name pointers 0x01374F54 / 0x01374F58. wchar_t is not a VC7.1 built-in here: the wide literal is `unsigned short`.
// Shape: the load failure returns the zero already in eax; the export call and the acceptance test nest inside positive
// `if`s so both failures share the `xor eax,eax` tail; `== 1` as a named bool (`dec ; neg ; sbb ; inc`) computed before the
// converted string's temporary is destroyed; FreeLibrary only on success (as retail).
extern "C" __declspec(dllimport) void* __stdcall LoadLibraryW(const unsigned short* fileName);                 // import 0x0143FE38
extern "C" __declspec(dllimport) void* __stdcall GetProcAddress(void* module, const char* procName);   // import 0x0143FE44
extern "C" __declspec(dllimport) int __stdcall FreeLibrary(void* module);                               // import 0x0143FE3C

class CCharStringData { public:
    const char*   Text;                     // +0x00
    unsigned long Length;                   // +0x04
};
class CCharString {
public:
    CCharStringData* PStringData;           // +0x00
    ~CCharString();                         // 0x0099EAE0
    const char* GetString() const { return PStringData == 0 ? "" : PStringData->Text; }
};
class CWideString {
public:
    void* PStringData;
    CCharString ConvertToCharString() const;   // 0x0099B2C0
};

extern CWideString g_RegistryKeyLocation;  // 0x013BCA28
extern const char* g_EULAFileName;          // 0x01374F54
extern const char* g_WarrantyFileName;      // 0x01374F58

typedef int (__stdcall* EBUEulaProc)(const char* regKeyLocation, const char* eulaFileName, const char* warrantyFileName, int checkForFirstRun);

namespace EULA {

int __fastcall DoEULAThings()
{
    void* library = LoadLibraryW(L"eula.dll");
    if (library == 0)
        return 0;
    EBUEulaProc eula = (EBUEulaProc)GetProcAddress(library, "EBUEula");
    if (eula != 0)
    {
        bool accepted = eula(g_RegistryKeyLocation.ConvertToCharString().GetString(), g_EULAFileName, g_WarrantyFileName, 1) == 1;
        if (accepted)
        {
            FreeLibrary(library);
            return 1;
        }
    }
    return 0;
}

}