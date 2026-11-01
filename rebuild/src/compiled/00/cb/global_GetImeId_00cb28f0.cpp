// GetImeId(unsigned int uIndex) -- DirectX SDK ImeUi.cpp: identify the current Chinese IME for the reading-window work-arounds: for the three legacy CHT/CHS keyboard layouts read the IME file name, accept the five known IME files (any file when the IME implements the reading-string API), take the file's version resource and cache (version | language, file version LS) for the known Traditional 4.2..6.0 / Simplified 4.1, 4.2, 5.3 versions; zero otherwise; cached per keyboard layout and returned by index
// Leaf: ?GetImeId@@YGKI@Z
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CB28F0, 662 bytes, one stack argument with callee cleanup (`ret 4`; a file static
//   in ImeUi.cpp; `__stdcall` with external linkage here, as the landed callers declare it -- a same-TU static gets a
//   register convention from VC7.1, uIndex in ebx), /GS frame
//   (cookie 0x0139C8A8, __security_check_cookie 0x00BFE9F9) for the `char szTmp[1024]` buffer.
// Identity (proven): FableWin.pdb + Ego_r.pdb name sets list ImeUi.cpp's static GetImeId; the body is the SDK's (DXSDK
//   2004/2005 ImeUi.cpp `static DWORD GetImeId( UINT uIndex )`: the _CHT_HKL 0xE0080404 / _CHT_HKL2 0xE0090404 /
//   _CHS_HKL 0xE00E0804 layouts, ImmGetIMEFileNameA, CompareStringA against _CHT_IMEFILENAME1..3 / _CHS_IMEFILENAME1..2
//   (0x012BFAC8 "TINTLGNT.IME", 0x012BFAB8 "CINTLGNT.IME", 0x012BFAA8 "MSTCIPHA.IME", 0x012BFA98 "PINTLGNT.IME",
//   0x012BFA88 "MSSCIPYA.IME"), GetFileVersionInfoSizeA / ImeUiCallback_Malloc / GetFileVersionInfoA / VerQueryValueA("\\"
//   0x0124C530) and the MAKEIMEVERSION table). This SDK revision returns dwRet[0] on success and zeroes both entries on every
//   failure (the June-2010 text keeps the old values). Callers: the landed CheckInputLocale 0x00CB37A0 and the reading-window
//   code 0x00CB31C0.. (`push 0 ; call`). The reloc binding ?GetImeId@@YGKI@Z is this spelling.
// Callees: ImmGetIMEFileNameA thunk 0x00CB5528, CompareStringA [0x0143FEF4] (hoisted into esi), GetFileVersionInfoSizeA thunk
//   0x00CB558E, GetFileVersionInfoA thunk 0x00CB5588, VerQueryValueA thunk 0x00CB5582, the callback pointers
//   ImeUiCallback_Malloc [0x0143E514] / ImeUiCallback_Free [0x0143CF08] (`__fastcall` pointers), __security_check_cookie.
// Globals: ImeUi.cpp file statics g_hklCurrent 0x0143E8B8 and _GetReadingString 0x0143E684 (DEFINED here as in the SDK TU:
//   VC7.1 then keeps g_hklCurrent and its LOWORD in registers across the calls, as retail -- an `extern` is reloaded), the
//   function statics hklPrev 0x0143E8D0 and dwRet[2] 0x0143E8C8. Variable names are the SDK source's (no PDB data symbols).
// Shape (the matching one differs from the public June-2010 SDK text in four places, each forced by the bytes): the layout /
//   ImmGetIMEFileNameA / file-name checks are one condition so every failure reaches the single zeroing tail at 0x00CB2B02
//   (separate early returns make VC7.1 materialise a zero register and duplicate the tail); `dwLang` is a DWORD
//   `(DWORD)g_hklCurrent & 0xffff` computed right after the hklPrev store (ebp: `mov ebp,eax ; and ebp,0xffff` at
//   0x00CB2952, `or eax,ebp` at 0x00CB2B52); the buffer checks are one condition and ImeUiCallback_Free is OUTSIDE the
//   lpVerBuffer test (the Malloc-failure edge 0x00CB2A59 `je 0x00CB2AF3` reaches `mov ecx,edi ; call Free`, i.e. retail
//   frees a null buffer; this is also what keeps uIndex in edi from the prologue and reloads it only after that Free);
//   success stores dwRet[0] / dwRet[1], frees and returns dwRet[0]. The version word is built as
//   `(dwVer & 0x00ff0000) << 8 | (dwVer & 0x000000ff) << 16`; the same-TU writer below (the SDK's SetImeApi start) keeps
//   the statics from folding to constants and gives GetImeId its same-TU caller.
struct HKL__ { int unused; };   typedef HKL__* HKL;
struct HIMC__ { int unused; };  typedef HIMC__* HIMC;
typedef unsigned long DWORD;
typedef unsigned int UINT;
typedef void* LPVOID;
typedef unsigned long LCID;
typedef unsigned char BYTE;

struct VS_FIXEDFILEINFO {
    DWORD dwSignature;                     // +0x00
    DWORD dwStrucVersion;                  // +0x04
    DWORD dwFileVersionMS;                 // +0x08
    DWORD dwFileVersionLS;                 // +0x0c
    DWORD dwProductVersionMS, dwProductVersionLS, dwFileFlagsMask, dwFileFlags, dwFileOS, dwFileType, dwFileSubtype,
          dwFileDateMS, dwFileDateLS;
};

extern "C" UINT __stdcall ImmGetIMEFileNameA(HKL hkl, char* lpszFileName, UINT uBufLen);                 // import thunk 0x00CB5528
extern "C" __declspec(dllimport) int __stdcall CompareStringA(LCID Locale, DWORD dwCmpFlags, const char* lpString1,
                                                              int cchCount1, const char* lpString2, int cchCount2);   // IAT 0x0143FEF4
extern "C" DWORD __stdcall GetFileVersionInfoSizeA(const char* lptstrFilename, DWORD* lpdwHandle);          // import thunk 0x00CB558E
extern "C" int __stdcall GetFileVersionInfoA(const char* lptstrFilename, DWORD dwHandle, DWORD dwLen, LPVOID lpData);   // thunk 0x00CB5588
extern "C" int __stdcall VerQueryValueA(const void* pBlock, const char* lpSubBlock, LPVOID* lplpBuffer, UINT* puLen);   // thunk 0x00CB5582

extern void* (__fastcall* ImeUiCallback_Malloc)(unsigned int bytes);   // 0x0143E514
extern void (__fastcall* ImeUiCallback_Free)(void* ptr);               // 0x0143CF08

// ImeUi.cpp file statics (defined in this TU as in the SDK source).
static unsigned int (__stdcall* _GetReadingString)(HIMC himc, unsigned int uReadingBufLen, unsigned short* lpwReadingBuf,
                                                   int* pnErrorIndex, int* pfIsVertical, unsigned int* puMaxReadingLen);  // 0x0143E684
static HKL g_hklCurrent;                                               // 0x0143E8B8

#define LANG_ENGLISH            0x09
#define SUBLANG_ENGLISH_US      0x01
#define SORT_DEFAULT            0x0
#define NORM_IGNORECASE         0x00000001
#define CSTR_EQUAL              2
#define MAKELANGID(p, s)        ((((unsigned short)(s)) << 10) | (unsigned short)(p))
#define MAKELCID(lgid, srtid)   ((DWORD)((((DWORD)((unsigned short)(srtid))) << 16) | ((DWORD)((unsigned short)(lgid)))))
#define GETLANG()               ((unsigned short)((DWORD)(g_hklCurrent) & 0xffff))
#define LANG_CHT                MAKELANGID(0x04, 0x01)       // 0x0404 Chinese Traditional
#define LANG_CHS                MAKELANGID(0x04, 0x02)       // 0x0804 Chinese Simplified
#define _CHT_HKL                ((HKL)(long)0xE0080404)      // New Phonetic
#define _CHT_HKL2               ((HKL)(long)0xE0090404)      // New Chang Jie
#define _CHS_HKL                ((HKL)(long)0xE00E0804)      // MSPY
#define _CHT_IMEFILENAME1       "TINTLGNT.IME"               // New Phonetic 5.1
#define _CHT_IMEFILENAME2       "CINTLGNT.IME"               // New Chang Jie 5.1
#define _CHT_IMEFILENAME3       "MSTCIPHA.IME"               // Phonetic 5.2
#define _CHS_IMEFILENAME1       "PINTLGNT.IME"               // MSPY1.5/2/3
#define _CHS_IMEFILENAME2       "MSSCIPYA.IME"               // MSPY3 for OfficeXP
#define MAKEIMEVERSION(major, minor)    ((DWORD)(((BYTE)(major) << 24) | ((BYTE)(minor) << 16)))

DWORD __stdcall GetImeId(UINT uIndex)
{
    static HKL hklPrev = 0;
    static DWORD dwRet[2] = { 0, 0 };

    DWORD dwVerSize;
    DWORD dwVerHandle;
    LPVOID lpVerBuffer;
    LPVOID lpVerData;
    UINT cbVerData;
    char szTmp[1024];

    if (uIndex >= sizeof(dwRet) / sizeof(dwRet[0]))
        return 0;

    if (hklPrev == g_hklCurrent)
        return dwRet[uIndex];

    hklPrev = g_hklCurrent;  // Save for the next invocation
    DWORD dwLang = (DWORD)g_hklCurrent & 0xffff;

    // Check if we are using an older Chinese IME; check if the system is NT; IME that doesn't implement Reading String API
    if ((g_hklCurrent == _CHT_HKL || g_hklCurrent == _CHT_HKL2 || g_hklCurrent == _CHS_HKL) &&
        ImmGetIMEFileNameA(g_hklCurrent, szTmp, (sizeof(szTmp) / sizeof(szTmp[0])) - 1) > 0 &&
        (_GetReadingString ||
         CompareStringA(MAKELCID(MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), SORT_DEFAULT), NORM_IGNORECASE, szTmp, -1, _CHT_IMEFILENAME1, -1) == CSTR_EQUAL ||
         CompareStringA(MAKELCID(MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), SORT_DEFAULT), NORM_IGNORECASE, szTmp, -1, _CHT_IMEFILENAME2, -1) == CSTR_EQUAL ||
         CompareStringA(MAKELCID(MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), SORT_DEFAULT), NORM_IGNORECASE, szTmp, -1, _CHT_IMEFILENAME3, -1) == CSTR_EQUAL ||
         CompareStringA(MAKELCID(MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), SORT_DEFAULT), NORM_IGNORECASE, szTmp, -1, _CHS_IMEFILENAME1, -1) == CSTR_EQUAL ||
         CompareStringA(MAKELCID(MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), SORT_DEFAULT), NORM_IGNORECASE, szTmp, -1, _CHS_IMEFILENAME2, -1) == CSTR_EQUAL))
    {
        dwVerSize = GetFileVersionInfoSizeA(szTmp, &dwVerHandle);
        if (dwVerSize)
        {
            lpVerBuffer = ImeUiCallback_Malloc(dwVerSize);
            if (lpVerBuffer &&
                GetFileVersionInfoA(szTmp, dwVerHandle, dwVerSize, lpVerBuffer) &&
                VerQueryValueA(lpVerBuffer, "\\", &lpVerData, &cbVerData))
            {
                DWORD dwVer = ((VS_FIXEDFILEINFO*)lpVerData)->dwFileVersionMS;
                dwVer = (dwVer & 0x00ff0000) << 8 | (dwVer & 0x000000ff) << 16;
                if (_GetReadingString ||
                    dwLang == LANG_CHT &&
                    (dwVer == MAKEIMEVERSION(4, 2) ||
                     dwVer == MAKEIMEVERSION(4, 3) ||
                     dwVer == MAKEIMEVERSION(4, 4) ||
                     dwVer == MAKEIMEVERSION(5, 0) ||
                     dwVer == MAKEIMEVERSION(5, 1) ||
                     dwVer == MAKEIMEVERSION(5, 2) ||
                     dwVer == MAKEIMEVERSION(6, 0)) ||
                    dwLang == LANG_CHS &&
                    (dwVer == MAKEIMEVERSION(4, 1) ||
                     dwVer == MAKEIMEVERSION(4, 2) ||
                     dwVer == MAKEIMEVERSION(5, 3)))
                {
                    dwRet[0] = dwVer | dwLang;
                    dwRet[1] = ((VS_FIXEDFILEINFO*)lpVerData)->dwFileVersionLS;
                    ImeUiCallback_Free(lpVerBuffer);
                    return dwRet[0];
                }
            }
            ImeUiCallback_Free(lpVerBuffer);   // retail frees a null buffer too (Malloc failure reaches the Free)
        }
    }
    dwRet[0] = dwRet[1] = 0;
    return dwRet[uIndex];
}

// Same-TU writer of the file statics (not graded): the SDK's SetImeApi start / CheckInputLocale write both statics and
// CheckInputLocale calls GetImeId; this keeps the TU shape (statics with a writer, GetImeId with a same-TU caller).
extern "C" __declspec(dllimport) HKL __stdcall GetKeyboardLayout(unsigned long idThread);   // [0x014403D8]
void __fastcall ImeUi_SameTuWriter()
{
    _GetReadingString = 0;
    g_hklCurrent = GetKeyboardLayout(0);
    GetImeId(0);
}