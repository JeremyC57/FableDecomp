// WinMainCRTStartup() -- VC7.1 CRT GUI entry point: managed check, CRT/argv init, skip program name, call WinMain
// Retail: Fable.exe (TLC, VC7.1) @ 0x00401067, 511 bytes (the PE entry point). crtexe.c built with _WINMAIN_ and
// _MBCS for the msvcr71 DLL model, size-optimised (/O1): the __try/__except frame uses the shared helpers
// __SEH_prolog 0x0040138C (push 74h ; push scopetable) / __SEH_epilog 0x004013C7.
// Shape: check_managed_app is inlined (DOS 'MZ' -> PE 'PE\0\0' -> optional-header magic 0x10B / 0x20B ->
// COM descriptor directory, entry 14); the argument cleanup of _initterm/atexit/__getmainargs is deferred into
// one `add esp,20h`; GetModuleHandleA stays cached in edi for the WinMain call; the filter's exception code
// is homed by the compiler at [ebp-2Ch] for the handler's GetExceptionCode().
// Callees: _RTC_Initialize 0x004012CE, _setargv 0x00401377, _matherr 0x00401374 (address only),
// _setdefaultprecision 0x0040135C, _initterm 0x00401356, atexit 0x004012BC (_RTC_Terminate 0x00401312),
// _amsg_exit 0x0040128A, WinMain 0x00403480, _XcptFilter 0x00401272; imports GetModuleHandleA [0x0143FE94],
// GetStartupInfoA [0x0143FE90], __set_app_type, __p__fmode, __p__commode, _adjust_fdiv, __setusermatherr,
// __getmainargs, _acmdln, exit, _cexit, _ismbblead, _exit, _c_exit.
// Leaf: _WinMainCRTStartup
// Flags: /O1

#include <excpt.h>

typedef void (__cdecl* _PVFV)(void);

struct _startupinfo
{
    int newmode;
};

struct STARTUPINFOA
{
    unsigned long cb;
    char* lpReserved;
    char* lpDesktop;
    char* lpTitle;
    unsigned long dwX;
    unsigned long dwY;
    unsigned long dwXSize;
    unsigned long dwYSize;
    unsigned long dwXCountChars;
    unsigned long dwYCountChars;
    unsigned long dwFillAttribute;
    unsigned long dwFlags;
    unsigned short wShowWindow;
    unsigned short cbReserved2;
    unsigned char* lpReserved2;
    void* hStdInput;
    void* hStdOutput;
    void* hStdError;
};

struct IMAGE_DOS_HEADER
{
    unsigned short e_magic;
    unsigned short e_cblp, e_cp, e_crlc, e_cparhdr, e_minalloc, e_maxalloc, e_ss, e_sp, e_csum, e_ip, e_cs,
        e_lfarlc, e_ovno, e_res[4], e_oemid, e_oeminfo, e_res2[10];
    long e_lfanew;
};

struct IMAGE_FILE_HEADER
{
    unsigned short Machine;
    unsigned short NumberOfSections;
    unsigned long TimeDateStamp;
    unsigned long PointerToSymbolTable;
    unsigned long NumberOfSymbols;
    unsigned short SizeOfOptionalHeader;
    unsigned short Characteristics;
};

struct IMAGE_DATA_DIRECTORY
{
    unsigned long VirtualAddress;
    unsigned long Size;
};

struct IMAGE_OPTIONAL_HEADER32
{
    unsigned short Magic;
    unsigned char MajorLinkerVersion, MinorLinkerVersion;
    unsigned long SizeOfCode, SizeOfInitializedData, SizeOfUninitializedData, AddressOfEntryPoint, BaseOfCode,
        BaseOfData, ImageBase, SectionAlignment, FileAlignment;
    unsigned short MajorOperatingSystemVersion, MinorOperatingSystemVersion, MajorImageVersion, MinorImageVersion,
        MajorSubsystemVersion, MinorSubsystemVersion;
    unsigned long Win32VersionValue, SizeOfImage, SizeOfHeaders, CheckSum;
    unsigned short Subsystem, DllCharacteristics;
    unsigned long SizeOfStackReserve, SizeOfStackCommit, SizeOfHeapReserve, SizeOfHeapCommit, LoaderFlags,
        NumberOfRvaAndSizes;
    IMAGE_DATA_DIRECTORY DataDirectory[16];
};

#pragma pack(push, 4)
struct IMAGE_OPTIONAL_HEADER64
{
    unsigned short Magic;
    unsigned char MajorLinkerVersion, MinorLinkerVersion;
    unsigned long SizeOfCode, SizeOfInitializedData, SizeOfUninitializedData, AddressOfEntryPoint, BaseOfCode;
    unsigned __int64 ImageBase;
    unsigned long SectionAlignment, FileAlignment;
    unsigned short MajorOperatingSystemVersion, MinorOperatingSystemVersion, MajorImageVersion, MinorImageVersion,
        MajorSubsystemVersion, MinorSubsystemVersion;
    unsigned long Win32VersionValue, SizeOfImage, SizeOfHeaders, CheckSum;
    unsigned short Subsystem, DllCharacteristics;
    unsigned __int64 SizeOfStackReserve, SizeOfStackCommit, SizeOfHeapReserve, SizeOfHeapCommit;
    unsigned long LoaderFlags, NumberOfRvaAndSizes;
    IMAGE_DATA_DIRECTORY DataDirectory[16];
};
#pragma pack(pop)

struct IMAGE_NT_HEADERS
{
    unsigned long Signature;
    IMAGE_FILE_HEADER FileHeader;
    IMAGE_OPTIONAL_HEADER32 OptionalHeader;
};

extern "C" __declspec(dllimport) void* __stdcall GetModuleHandleA(const char* moduleName);   // [0x0143FE94]
extern "C" __declspec(dllimport) void __stdcall GetStartupInfoA(STARTUPINFOA* info);         // [0x0143FE90]
extern "C" __declspec(dllimport) void __cdecl __set_app_type(int appType);                   // [0x0144017C]
extern "C" __declspec(dllimport) int* __cdecl __p__fmode(void);                              // [0x01440180]
extern "C" __declspec(dllimport) int* __cdecl __p__commode(void);                            // [0x01440184]
extern "C" int* _imp___adjust_fdiv;                                                         // [0x01440188]
extern "C" __declspec(dllimport) void __cdecl __setusermatherr(int (__cdecl* f)(struct _exception*));   // [0x0144018C]
extern "C" __declspec(dllimport) int __cdecl __getmainargs(int* argc, char*** argv, char*** envp, int doWildCard,
                                                           _startupinfo* startInfo);         // [0x01440194]
extern "C" __declspec(dllimport) char* _acmdln;                                              // [0x0144019C]
extern "C" __declspec(dllimport) __declspec(noreturn) void __cdecl exit(int code);                               // [0x014401A0]
extern "C" __declspec(dllimport) void __cdecl _cexit(void);                                  // [0x014401A4]
extern "C" __declspec(dllimport) int __cdecl _ismbblead(unsigned int c);                     // [0x014401A8]
extern "C" __declspec(dllimport) __declspec(noreturn) void __cdecl _exit(int code);                              // [0x014401B0]
extern "C" __declspec(dllimport) void __cdecl _c_exit(void);                                 // [0x014400B8]

extern "C" void __cdecl _RTC_Initialize(void);          // 0x004012CE
extern "C" void __cdecl _RTC_Terminate(void);           // 0x00401312
extern "C" int __cdecl _setargv(void);                  // 0x00401377
extern "C" int __cdecl _matherr(struct _exception* e);  // 0x00401374
extern "C" void __cdecl _setdefaultprecision(void);     // 0x0040135C
extern "C" void __cdecl _initterm(_PVFV* first, _PVFV* last);   // 0x00401356
extern "C" int __cdecl atexit(_PVFV func);              // 0x004012BC
extern "C" void __cdecl _amsg_exit(int code);           // 0x0040128A
extern "C" int __cdecl _XcptFilter(unsigned long code, struct _EXCEPTION_POINTERS* info);   // 0x00401272
extern "C" int __stdcall WinMain(void* instance, void* previousInstance, char* commandLine, int showCommand);   // 0x00403480

extern "C" _PVFV* __onexitbegin;     // 0x0143E9A0
extern "C" _PVFV* __onexitend;       // 0x0143E990
extern "C" int _fmode;               // 0x013B7C7C
extern "C" int _commode;             // 0x013B7C78
extern "C" int _adjust_fdiv;         // 0x0143E984
extern "C" int __defaultmatherr;     // 0x01374F50
extern "C" int _newmode;             // 0x013B7C74
extern "C" int _dowildcard;          // 0x013B7C70
extern "C" _PVFV __xi_a[];           // 0x01374D3C
extern "C" _PVFV __xi_z[];           // 0x01374E40
extern "C" _PVFV __xc_a[];           // 0x01374000
extern "C" _PVFV __xc_z[];           // 0x01374C38

static __forceinline int __cdecl check_managed_app(void)
{
    IMAGE_DOS_HEADER* pDOSHeader;
    IMAGE_NT_HEADERS* pPEHeader;
    IMAGE_OPTIONAL_HEADER32* pNTHeader32;
    IMAGE_OPTIONAL_HEADER64* pNTHeader64;

    pDOSHeader = (IMAGE_DOS_HEADER*)GetModuleHandleA(0);
    if (pDOSHeader->e_magic != 0x5A4D)
        return 0;

    pPEHeader = (IMAGE_NT_HEADERS*)((char*)pDOSHeader + pDOSHeader->e_lfanew);
    if (pPEHeader->Signature != 0x4550)
        return 0;

    pNTHeader32 = (IMAGE_OPTIONAL_HEADER32*)&pPEHeader->OptionalHeader;
    switch (pNTHeader32->Magic) {
    case 0x10B:
        if (pNTHeader32->NumberOfRvaAndSizes <= 14)
            return 0;
        return !!pNTHeader32->DataDirectory[14].VirtualAddress;
    case 0x20B:
        pNTHeader64 = (IMAGE_OPTIONAL_HEADER64*)pNTHeader32;
        if (pNTHeader64->NumberOfRvaAndSizes <= 14)
            return 0;
        return !!pNTHeader64->DataDirectory[14].VirtualAddress;
    }
    return 0;
}

extern "C" int WinMainCRTStartup(void)
{
    int argc;
    char** argv;
    char** envp;
    int argret;
    int mainret;
    int managedapp;
    unsigned char* lpszCommandLine;
    STARTUPINFOA StartupInfo;
    int inDoubleQuote = 0;
    _startupinfo startinfo;

    managedapp = check_managed_app();

    __try {
        __set_app_type(2);

        __onexitbegin = __onexitend = (_PVFV*)(-1);

        *__p__fmode() = _fmode;
        *__p__commode() = _commode;

        _adjust_fdiv = *_imp___adjust_fdiv;

        _RTC_Initialize();

        _setargv();

        if (!__defaultmatherr)
            __setusermatherr(_matherr);

        _setdefaultprecision();

        _initterm(__xi_a, __xi_z);

        atexit(_RTC_Terminate);

        startinfo.newmode = _newmode;

        argret = __getmainargs(&argc, &argv, &envp, _dowildcard, &startinfo);

        if (argret < 0)
            _amsg_exit(8);

        _initterm(__xc_a, __xc_z);

        lpszCommandLine = (unsigned char*)_acmdln;

        while (*lpszCommandLine > ' ' || (*lpszCommandLine && inDoubleQuote)) {
            if (*lpszCommandLine == '"')
                inDoubleQuote = !inDoubleQuote;
            if (_ismbblead(*lpszCommandLine)) {
                if (lpszCommandLine)
                    lpszCommandLine++;
            }
            ++lpszCommandLine;
        }

        while (*lpszCommandLine && (*lpszCommandLine <= ' '))
            lpszCommandLine++;

        StartupInfo.dwFlags = 0;
        GetStartupInfoA(&StartupInfo);

        mainret = WinMain(GetModuleHandleA(0), 0, (char*)lpszCommandLine,
                          StartupInfo.dwFlags & 1 ? StartupInfo.wShowWindow : 10);

        if (!managedapp)
            exit(mainret);

        _cexit();
    }
    __except (_XcptFilter(GetExceptionCode(), (struct _EXCEPTION_POINTERS*)GetExceptionInformation())) {
        mainret = GetExceptionCode();

        if (!managedapp)
            _exit(mainret);

        _c_exit();
    }

    return mainret;
}