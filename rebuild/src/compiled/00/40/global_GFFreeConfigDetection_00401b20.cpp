// _global::GFFreeConfigDetection() -- release the hardware-config detection DLL: call its optional "Config_Release" export, FreeLibrary the module, clear the handle, return false
// Leaf: GFFreeConfigDetection
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00401B20, 49 bytes. Callees: kernel32 GetProcAddress
// (import slot 0x0143FE44), FreeLibrary (import slot 0x0143FE3C); module handle global 0x013B7C8C,
// export name literal 0x0122D12C ("Config_Release").
//
// Retail bytes:  mov eax,[g_module] ; push "Config_Release" ; push eax ; call [GetProcAddress] ;
//   test eax,eax ; je N ; call eax ; N: mov ecx,[g_module] ; push ecx ; call [FreeLibrary] ;
//   mov [g_module],0 ; xor al,al ; ret
// The counterpart of GFConfigDetection (which LoadLibraryA's the module into the same global).
// The release export takes no arguments; the handle is re-read after the call (the export may
// run arbitrary code), and the bool result is the 8-bit `xor al,al` false. An earlier lane landed
// this body as Global_GFFreeConfigDetection_00401b20.cpp (fable_config_detection.h); this TU is
// self-contained with the two kernel32 imports declared dllimport.
struct HINSTANCE__;
typedef HINSTANCE__* HMODULE;
typedef int (__stdcall *FARPROC)();

extern "C" __declspec(dllimport) FARPROC __stdcall GetProcAddress(HMODULE module, const char* name);   // [0x0143FE44]
extern "C" __declspec(dllimport) int __stdcall FreeLibrary(HMODULE module);                          // [0x0143FE3C]

typedef void (__stdcall *PFNConfigRelease)();

extern HMODULE g_hConfigDetectionModule;          // 0x013B7C8C

bool __fastcall GFFreeConfigDetection()
{
    PFNConfigRelease release = (PFNConfigRelease)GetProcAddress(g_hConfigDetectionModule, "Config_Release");
    if (release)
        release();
    FreeLibrary(g_hConfigDetectionModule);
    g_hConfigDetectionModule = 0;
    return false;
}