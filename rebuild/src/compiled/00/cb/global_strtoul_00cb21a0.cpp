// _strtoul(const char* psz, char** ppEnd, int nBase) -- DXUT ImeUi.cpp: parse a "0x"-prefixed hexadecimal or a decimal unsigned number (the end pointer and base are ignored)
// Leaf: ?_strtoul@@YAKPBDPAPADH@Z
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CB21A0, 143 bytes, a TU-local static (or API) of the same library file as the landed
//   row 0x00CB37A0. Shape (retry1): parked as an "LTCG custom register convention" gap; the landed host TU already holds this
//   function's real body inside `extern "C" { static __declspec(noinline) }` beside its real caller, and graded with this
//   leaf it matches retail as is (VC7.1 /O2 picks the private convention itself). The source is the host TU verbatim.
// TU host (landed row 0x00CB37A0): CheckInputLocale() -- DirectX SDK ImeUi.cpp: when the keyboard layout changes, pick the language indicator and candidate-list orientation for it and rebuild the DBCS lead-byte table for its ANSI code page
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CB37A0, 280 bytes, __fastcall (ImeUi.cpp is a /Gr TU), no arguments, /GS frame
//   (cookie 0x0139C8A8, __security_check_cookie 0x00BFE9F9) for the `char szCodePage[8]` buffer.
// Identity: FableWin.pdb + Ego_r.pdb name sets list ImeUi.cpp's statics CheckInputLocale, GetImeId and _strtoul; the landed
//   CheckToggleState 0x00CB47C0 calls this row first (`void __fastcall CheckInputLocale()`, bound ?CheckInputLocale@@YIXXZ).
//   Body: the SDK CheckInputLocale -- GetKeyboardLayout(0) against the function-static hklPrev, the LANG_CHINESE (sublanguage
//   TRADITIONAL 1 / SIMPLIFIED 2) / LANG_JAPANESE / LANG_KOREAN switch over the g_aszIndicator wide strings (L"A", L"\x4E2D",
//   L"\x4E2D", L"\xAC00", L"\x3042" in .data at 0x013AC74C), then GetLocaleInfoA(LOCALE_IDEFAULTANSICODEPAGE) -> _strtoul ->
//   IsDBCSLeadByteEx for all 256 bytes.
// Callees: GetKeyboardLayout [0x014403D8], GetImeId 0x00CB28F0 (a static with a private callee-pop convention: `push 0 ;
//   call`, no cleanup -- declared __stdcall here, the same call site), GetLocaleInfoA [0x0143FEEC], _strtoul 0x00CB21A0
//   (same-TU static; its private convention takes psz in edx, the unused base/endptr arguments dropped), IsDBCSLeadByteEx
//   [0x0143FEF0] (hoisted into edi).
// Globals: g_hklCurrent 0x0143E8B8, hklPrev 0x0143E8D8 (function static), g_bVerticalCand 0x013AC748 (initialised true),
//   g_aszIndicator 0x013AC74C, g_pszIndicatior 0x0143E8C0 (dynamic-initialised to g_aszIndicator[0] by 0x01224C40),
//   g_uCodePage 0x0143E8BC, LeadByteTable 0x0143CE08. Variable names are the SDK source's (no PDB data symbols).
struct HKL__ { int unused; };   typedef HKL__* HKL;

extern "C" __declspec(dllimport) HKL __stdcall GetKeyboardLayout(unsigned long idThread);                  // [0x014403D8]
extern "C" __declspec(dllimport) int __stdcall GetLocaleInfoA(unsigned long Locale, unsigned long LCType,
                                                              char* lpLCData, int cchData);              // [0x0143FEEC]
extern "C" __declspec(dllimport) int __stdcall IsDBCSLeadByteEx(unsigned int CodePage, unsigned char TestChar); // [0x0143FEF0]

unsigned long __stdcall GetImeId(unsigned int uIndex = 0);   // 0x00CB28F0 (static in retail, private callee-pop convention)

extern HKL                   g_hklCurrent;          // 0x0143E8B8
extern bool                  g_bVerticalCand;       // 0x013AC748
extern const unsigned short* g_aszIndicator[5];     // 0x013AC74C
extern const unsigned short* g_pszIndicatior;       // 0x0143E8C0
extern unsigned int          g_uCodePage;           // 0x0143E8BC
extern unsigned char         LeadByteTable[256];    // 0x0143CE08

#define LANG_CHINESE                0x04
#define LANG_JAPANESE               0x11
#define LANG_KOREAN                 0x12
#define SUBLANG_CHINESE_TRADITIONAL 0x01
#define SUBLANG_CHINESE_SIMPLIFIED  0x02
#define SORT_DEFAULT                0x0
#define LOCALE_IDEFAULTANSICODEPAGE 0x00001004
#define MAKELCID(lgid, srtid)  ((unsigned long)((((unsigned long)((unsigned short)(srtid))) << 16) | ((unsigned long)((unsigned short)(lgid)))))
#define GETLANG()       ((unsigned short)((unsigned long)(g_hklCurrent) & 0xffff))
#define GETPRIMLANG()   ((unsigned short)(GETLANG() & 0x3ff))
#define GETSUBLANG()    ((unsigned short)(GETLANG() >> 10))
#define COUNTOF(a)      (sizeof(a) / sizeof((a)[0]))

#define INDICATOR_NON_IME   0
#define INDICATOR_CHS       1
#define INDICATOR_CHT       2
#define INDICATOR_KOREAN    3
#define INDICATOR_JAPANESE  4

// ImeUi.cpp's own hex/decimal parser (0x00CB21A0); the endptr/base parameters are unused.
static unsigned long _strtoul(const char* psz, char**, int)
{
    if (!psz)
        return 0;

    unsigned long ulRet = 0;
    if (psz[0] == '0' && (psz[1] == 'x' || psz[1] == 'X'))
    {
        psz += 2;
        unsigned long ul = 0;
        while (*psz)
        {
            if ('0' <= *psz && *psz <= '9')
                ul = *psz - '0';
            else if ('A' <= *psz && *psz <= 'F')
                ul = *psz - 'A' + 10;
            else if ('a' <= *psz && *psz <= 'f')
                ul = *psz - 'a' + 10;
            else
                break;
            ulRet = ulRet * 16 + ul;
            psz++;
        }
    }
    else
    {
        while (*psz && ('0' <= *psz && *psz <= '9'))
        {
            ulRet = ulRet * 10 + (*psz - '0');
            psz++;
        }
    }
    return ulRet;
}

void __fastcall CheckInputLocale()
{
    static HKL hklPrev = 0;
    g_hklCurrent = GetKeyboardLayout(0);
    if (hklPrev == g_hklCurrent)
    {
        return;
    }
    hklPrev = g_hklCurrent;
    switch (GETPRIMLANG())
    {
    // Simplified Chinese
    case LANG_CHINESE:
        g_bVerticalCand = true;
        switch (GETSUBLANG())
        {
        case SUBLANG_CHINESE_SIMPLIFIED:
            g_pszIndicatior = g_aszIndicator[INDICATOR_CHS];
            g_bVerticalCand = GetImeId() == 0;
            break;
        case SUBLANG_CHINESE_TRADITIONAL:
            g_pszIndicatior = g_aszIndicator[INDICATOR_CHT];
            break;
        default:    // unsupported sub-language
            g_pszIndicatior = g_aszIndicator[INDICATOR_NON_IME];
            break;
        }
        break;
    // Korean
    case LANG_KOREAN:
        g_pszIndicatior = g_aszIndicator[INDICATOR_KOREAN];
        g_bVerticalCand = false;
        break;
    // Japanese
    case LANG_JAPANESE:
        g_pszIndicatior = g_aszIndicator[INDICATOR_JAPANESE];
        g_bVerticalCand = true;
        break;
    default:
        g_pszIndicatior = g_aszIndicator[INDICATOR_NON_IME];
    }
    char szCodePage[8];
    int iRc = GetLocaleInfoA(MAKELCID(GETLANG(), SORT_DEFAULT), LOCALE_IDEFAULTANSICODEPAGE, szCodePage, COUNTOF(szCodePage)); iRc;
    g_uCodePage = _strtoul(szCodePage, 0, 16);
    for (int i = 0; i < 256; i++)
    {
        LeadByteTable[i] = (unsigned char)IsDBCSLeadByteEx(g_uCodePage, (unsigned char)i);
    }
}