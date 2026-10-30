// DetectWindowsVersion() -- GameCODA platform probe: split GetVersion() into major/minor/build and raise one flag for the host (Win32s, 95, 98, 98 SE, Me, NT 3, NT 4, 2000, XP), then mark the probe done
// Grade: DIFFER(260v262) -- the NT switch: retail copies the major version to eax (`mov eax,ecx ; sub eax,3 ; dec eax ; dec eax`), VC7.1 here switches on ecx in place; everything else matches
// Leaf: ?DetectWindowsVersion@@YAXXZ
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C73660, 262 bytes, plain `ret`; /GS frame (cookie 0x0139C8A8, check 0x00BFE9F9)
//   around the 0x94-byte OSVERSIONINFOA. Imports: GetVersion (IAT 0x0143FECC), GetVersionExA (IAT 0x0143FE70).
// Caller: the platform-info constructor 0x00C73770 (`if (!g_windowsVersionDetected) DetectWindowsVersion(); return this;`),
//   built on the stack by the DirectSound set-up 0x00C5DD70, which then reads the 9x flags directly (and through
//   IsWindows9x 0x00C5DA70). The function never touches ecx, so it is modelled as a free function.
// Name: descriptive. GameCODA/libpk has no PDB or RTTI; the landed spelling for 0x00C73660,
//   SetDefaultString_helper(CDefString*) __fastcall, has no evidence (no string, no argument, no result) -- NAME_FIXES.tsv.
// Shape: the version word is split into locals (major in ecx, minor in edx) stored to the globals at once; the NT
//   branch switches on the local major (sub 3 / dec / dec); the 9x branch re-reads the minor global after
//   GetVersionExA. Every flag store uses the `true` constant VC7.1 keeps in ebx.
struct _OSVERSIONINFOA {
    unsigned long dwOSVersionInfoSize;
    unsigned long dwMajorVersion;
    unsigned long dwMinorVersion;
    unsigned long dwBuildNumber;
    unsigned long dwPlatformId;
    char          szCSDVersion[128];
};

extern "C" __declspec(dllimport) unsigned long __stdcall GetVersion(void);                  // IAT 0x0143FECC
extern "C" __declspec(dllimport) int __stdcall GetVersionExA(_OSVERSIONINFOA* lpVersionInfo); // IAT 0x0143FE70

extern "C" unsigned char g_defStringInit;   // 0x0143CCC0 (bound spelling; really "version probe done")
extern bool g_isWin32s;                     // 0x0143CCC1
extern bool g_isWindows95;                  // 0x0143CCC2
extern bool g_isWindows98;                  // 0x0143CCC3
extern bool g_isWindows98SE;                // 0x0143CCC4
extern bool g_isWindowsMe;                  // 0x0143CCC5
extern bool g_isWindowsNT3;                 // 0x0143CCC6
extern bool g_isWindowsNT4;                 // 0x0143CCC7
extern bool g_isWindows2000;                // 0x0143CCC8
extern bool g_isWindowsXP;                  // 0x0143CCC9
extern unsigned long g_windowsBuildNumber;  // 0x0143CCCC
extern unsigned long g_windowsMajorVersion; // 0x0143CCD0
extern unsigned long g_windowsMinorVersion; // 0x0143CCD4

void DetectWindowsVersion()
{
    unsigned long version = GetVersion();
    unsigned long major = (unsigned char)version;
    unsigned long minor = (unsigned char)((unsigned short)version >> 8);
    g_windowsMajorVersion = major;
    g_windowsMinorVersion = minor;

    if (version < 0x80000000) {
        // Windows NT family
        g_windowsBuildNumber = version >> 16;
        switch (major) {
        case 3:
            g_isWindowsNT3 = true;
            break;
        case 4:
            g_isWindowsNT4 = true;
            break;
        case 5:
            if (minor == 0)
                g_isWindows2000 = true;
            else if (minor == 1)
                g_isWindowsXP = true;
            break;
        }
    } else if (major < 4) {
        g_windowsBuildNumber = (version >> 16) & 0x7FFF;
        g_isWin32s = true;
    } else {
        _OSVERSIONINFOA info;
        info.dwOSVersionInfoSize = sizeof(info);
        GetVersionExA(&info);
        g_windowsBuildNumber = (unsigned short)info.dwBuildNumber;
        if (g_windowsMinorVersion == 0)
            g_isWindows95 = true;
        else if (g_windowsMinorVersion <= 10 && g_windowsBuildNumber <= 2222) {
            if (g_windowsBuildNumber == 2222)
                g_isWindows98SE = true;
            else
                g_isWindows98 = true;
        } else
            g_isWindowsMe = true;
    }
    g_defStringInit = 1;
}
