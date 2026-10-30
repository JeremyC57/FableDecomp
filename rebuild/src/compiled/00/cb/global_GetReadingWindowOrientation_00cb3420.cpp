// GetReadingWindowOrientation(unsigned long dwId) -- DirectX SDK ImeUi.cpp: decide whether the IME reading window is horizontal: true for MSPY (CHS), New Chang Jie or an unknown IME id; for other Traditional Chinese IMEs read HKCU\software\microsoft\windows\currentversion\{MSTCIPH|TINTLGNT} "keyboard mapping" and go horizontal for the version-specific mappings 0x22..0x24
// Leaf: ?GetReadingWindowOrientation@@YGXK@Z
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CB3420, 333 bytes, callee-pop `ret 4` with the id on the stack (ImeUi.cpp is a /Gr TU;
//   the static gets the same private callee-pop convention as the landed GetImeId 0x00CB28F0, declared __stdcall), /GS frame
//   0x118 for `char szRegPath[MAX_PATH]` (cookie 0x0139C8A8, __security_check_cookie 0x00BFE9F9).
// Identity: FableWin.pdb + Ego_r.pdb name sets both list ImeUi.cpp's static GetReadingWindowOrientation (next to GetImeId and
//   CheckInputLocale); the body is the SDK one, proven by its strings ("software\\microsoft\\windows\\currentversion\\"
//   0x012BFB10, "MSTCIPH" 0x012BFB08, "TINTLGNT" 0x012BFAFC, "keyboard mapping" 0x012BFAE8) and the _CHS_HKL 0xE00E0804 /
//   _CHT_HKL_NEW_CHANG_JIE 0xE0090404 compares. Its only caller is ImeUi.cpp's GetPrivateReadingString path (call 0x00CB472A,
//   which then tests g_bHorizontalReading). The manifest label is the same name.
// Callees: lstrcpyA [0x0143FD80], lstrcatA [0x0143FD84], RegOpenKeyExA [0x0143FC08], RegQueryValueExA [0x0143FBEC],
//   RegCloseKey [0x0143FBFC]. Globals: g_hklCurrent 0x0143E8B8, g_bHorizontalReading 0x0143E8A1 (ImeUi.cpp file statics,
//   SDK names; no PDB data symbols).
// Shape: the SDK body verbatim. It matches only as a TU-local `static __declspec(noinline)` with a same-TU caller (as in
//   ImeUi.cpp): declared extern __stdcall it compiles to 330 bytes (g_hklCurrent in eax, dwId loaded late). The caller here
//   is a reduced stand-in for ImeUi.cpp's static GetReadingString 0x00CB4340 (the PDB name sets list it; GetImeId first,
//   then this call); only the GetReadingWindowOrientation leaf is graded.
struct HKL__ { int unused; };   typedef HKL__* HKL;
struct HKEY__ { int unused; };  typedef HKEY__* HKEY;
typedef unsigned long DWORD;
typedef unsigned char BYTE;
typedef long LONG;

extern "C" __declspec(dllimport) char* __stdcall lstrcpyA(char* lpString1, const char* lpString2);          // [0x0143FD80]
extern "C" __declspec(dllimport) char* __stdcall lstrcatA(char* lpString1, const char* lpString2);          // [0x0143FD84]
extern "C" __declspec(dllimport) LONG __stdcall RegOpenKeyExA(HKEY hKey, const char* lpSubKey, DWORD ulOptions,
                                                              DWORD samDesired, HKEY* phkResult);          // [0x0143FC08]
extern "C" __declspec(dllimport) LONG __stdcall RegQueryValueExA(HKEY hKey, const char* lpValueName, DWORD* lpReserved,
                                                                 DWORD* lpType, BYTE* lpData, DWORD* lpcbData);   // [0x0143FBEC]
extern "C" __declspec(dllimport) LONG __stdcall RegCloseKey(HKEY hKey);                                    // [0x0143FBFC]

struct HWND__ { int unused; };  typedef HWND__* HWND;
DWORD __stdcall GetImeId(unsigned int uIndex = 0);   // 0x00CB28F0 (static in retail, private callee-pop convention)

extern HKL  g_hklCurrent;           // 0x0143E8B8
extern bool g_bHorizontalReading;   // 0x0143E8A1

#define MAX_PATH                260
#define ERROR_SUCCESS           0L
#define KEY_READ                0x20019
#define HKEY_CURRENT_USER       ((HKEY)(unsigned long)0x80000001)
#define LANG_CHT                0x0404           // MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL)
#define _CHT_HKL_NEW_CHANG_JIE  ((HKL)0xE0090404)
#define _CHS_HKL                ((HKL)0xE00E0804)
#define MAKEIMEVERSION(major, minor) ((DWORD)(((BYTE)(major) << 24) | ((BYTE)(minor) << 16)))
#define IMEID_VER(id)           ((id) & 0xffff0000)
#define IMEID_LANG(id)          ((id) & 0x0000ffff)

static __declspec(noinline) void __stdcall GetReadingWindowOrientation(DWORD dwId)
{
    g_bHorizontalReading = (g_hklCurrent == _CHS_HKL) || (g_hklCurrent == _CHT_HKL_NEW_CHANG_JIE) || (dwId == 0);
    if (!g_bHorizontalReading && IMEID_LANG(dwId) == LANG_CHT)
    {
        char szRegPath[MAX_PATH];
        HKEY hkey;
        DWORD dwVer = IMEID_VER(dwId);
        lstrcpyA(szRegPath, "software\\microsoft\\windows\\currentversion\\");
        lstrcatA(szRegPath, (dwVer >= MAKEIMEVERSION(5, 1)) ? "MSTCIPH" : "TINTLGNT");
        LONG lRc = RegOpenKeyExA(HKEY_CURRENT_USER, szRegPath, 0, KEY_READ, &hkey);
        if (lRc == ERROR_SUCCESS)
        {
            DWORD dwSize = sizeof(DWORD), dwMapping, dwType;
            lRc = RegQueryValueExA(hkey, "keyboard mapping", 0, &dwType, (BYTE*)&dwMapping, &dwSize);
            if (lRc == ERROR_SUCCESS)
            {
                if (
                    (dwVer <= MAKEIMEVERSION(5, 0) &&
                     ((BYTE)dwMapping == 0x22 || (BYTE)dwMapping == 0x23)) ||
                    ((dwVer == MAKEIMEVERSION(5, 1) || dwVer == MAKEIMEVERSION(5, 2)) &&
                     (BYTE)dwMapping >= 0x22 && (BYTE)dwMapping <= 0x24)
                   )
                {
                    g_bHorizontalReading = true;
                }
            }
            RegCloseKey(hkey);
        }
    }
}

// Reduced stand-in for ImeUi.cpp's GetReadingString (0x00CB4340, static in retail; hWnd on the stack): it gives the static
// above its same-TU caller. Not part of the graded leaf.
void __stdcall GetReadingString(HWND hWnd)
{
    DWORD dwId = GetImeId();
    if (!dwId)
        return;
    GetReadingWindowOrientation(dwId);
}