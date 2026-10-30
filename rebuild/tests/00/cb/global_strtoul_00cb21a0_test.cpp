// Behaviour test for 00cb21a0 (adapted from the landed host row 0x00cb37a0's test, whose TU exercises this function through its real caller).
// Behaviour test for 0x00CB37A0 ImeUi.cpp CheckInputLocale: indicator / vertical-candidate choice per language, the
// unchanged-layout early exit, and the code page parse + lead-byte table rebuild (Win32 calls stubbed).
#include <cstdio>
#include <cstring>

struct HKL__ { int unused; };   typedef HKL__* HKL;

static unsigned long g_layout = 0;
static int g_layoutCalls = 0, g_localeCalls = 0, g_imeId = 0;
static unsigned long g_lastLcid = 0;
static const char* g_codePageText = "936";
static HKL GetKeyboardLayout(unsigned long) { ++g_layoutCalls; return (HKL)g_layout; }
static int GetLocaleInfoA(unsigned long lcid, unsigned long type, char* buf, int cch)
{
    ++g_localeCalls; g_lastLcid = lcid;
    if (type != 0x1004 || cch != 8) return 0;
    std::strcpy(buf, g_codePageText);
    return (int)std::strlen(buf) + 1;
}
static int IsDBCSLeadByteEx(unsigned int cp, unsigned char c) { return cp == 936 && c >= 0x81 && c <= 0xFE; }
static unsigned long GetImeId(unsigned int = 0) { return (unsigned long)g_imeId; }

static const unsigned short s_A[] = { 'A', 0 }, s_zh[] = { 0x4E2D, 0 }, s_ko[] = { 0xAC00, 0 }, s_ja[] = { 0x3042, 0 };
static HKL g_hklCurrent;
static bool g_bVerticalCand = true;
static const unsigned short* g_aszIndicator[5] = { s_A, s_zh, s_zh, s_ko, s_ja };
static const unsigned short* g_pszIndicatior = g_aszIndicator[0];
static unsigned int g_uCodePage;
static unsigned char LeadByteTable[256];

#define GETLANG()       ((unsigned short)((unsigned long)(g_hklCurrent) & 0xffff))
#define GETPRIMLANG()   ((unsigned short)(GETLANG() & 0x3ff))
#define GETSUBLANG()    ((unsigned short)(GETLANG() >> 10))

static unsigned long _strtoul(const char* psz, char**, int)
{
    if (!psz) return 0;
    unsigned long ulRet = 0;
    if (psz[0] == '0' && (psz[1] == 'x' || psz[1] == 'X')) {
        psz += 2;
        unsigned long ul = 0;
        while (*psz) {
            if ('0' <= *psz && *psz <= '9') ul = *psz - '0';
            else if ('A' <= *psz && *psz <= 'F') ul = *psz - 'A' + 10;
            else if ('a' <= *psz && *psz <= 'f') ul = *psz - 'a' + 10;
            else break;
            ulRet = ulRet * 16 + ul;
            psz++;
        }
    }
    else {
        while (*psz && ('0' <= *psz && *psz <= '9')) { ulRet = ulRet * 10 + (*psz - '0'); psz++; }
    }
    return ulRet;
}

static void CheckInputLocale()
{
    static HKL hklPrev = 0;
    g_hklCurrent = GetKeyboardLayout(0);
    if (hklPrev == g_hklCurrent) return;
    hklPrev = g_hklCurrent;
    switch (GETPRIMLANG()) {
    case 0x04:
        g_bVerticalCand = true;
        switch (GETSUBLANG()) {
        case 0x02: g_pszIndicatior = g_aszIndicator[1]; g_bVerticalCand = GetImeId() == 0; break;
        case 0x01: g_pszIndicatior = g_aszIndicator[2]; break;
        default:   g_pszIndicatior = g_aszIndicator[0]; break;
        }
        break;
    case 0x12: g_pszIndicatior = g_aszIndicator[3]; g_bVerticalCand = false; break;
    case 0x11: g_pszIndicatior = g_aszIndicator[4]; g_bVerticalCand = true; break;
    default:   g_pszIndicatior = g_aszIndicator[0];
    }
    char szCodePage[8];
    GetLocaleInfoA((unsigned long)GETLANG(), 0x1004, szCodePage, 8);
    g_uCodePage = _strtoul(szCodePage, 0, 16);
    for (int i = 0; i < 256; i++) LeadByteTable[i] = (unsigned char)IsDBCSLeadByteEx(g_uCodePage, (unsigned char)i);
}

int main()
{
    int bad = 0;
    g_layout = 0x08040804; g_imeId = 0;                       // zh-CN, IME id 0: vertical candidates
    CheckInputLocale();
    bad |= g_pszIndicatior != s_zh || !g_bVerticalCand || g_uCodePage != 936 || g_lastLcid != 0x0804
           || LeadByteTable[0x80] != 0 || LeadByteTable[0x81] != 1 || LeadByteTable[0xFE] != 1 || LeadByteTable[0xFF] != 0;
    int localeCalls = g_localeCalls;
    CheckInputLocale();                                       // same layout: early exit
    bad |= g_localeCalls != localeCalls || g_layoutCalls != 2;
    g_layout = 0x08040804 + 0x10000; g_imeId = 5;             // still zh-CN language word: early exit only if HKL equal
    CheckInputLocale();
    bad |= g_pszIndicatior != s_zh || g_bVerticalCand;        // IME id != 0: horizontal
    g_layout = 0x04110411; g_codePageText = "932";            // ja-JP
    CheckInputLocale();
    bad |= g_pszIndicatior != s_ja || !g_bVerticalCand || g_uCodePage != 932 || LeadByteTable[0x81] != 0;
    g_layout = 0x04120412; g_codePageText = "0x3B5";          // ko-KR, hex code page text
    CheckInputLocale();
    bad |= g_pszIndicatior != s_ko || g_bVerticalCand || g_uCodePage != 949;
    g_layout = 0x04040404; g_codePageText = "950";            // zh-TW
    CheckInputLocale();
    bad |= g_pszIndicatior != s_zh || !g_bVerticalCand;
    g_layout = 0x0C040C04;                                    // zh-HK (sublanguage 3): non-IME indicator, vertical stays set
    CheckInputLocale();
    bad |= g_pszIndicatior != s_A || !g_bVerticalCand;
    g_layout = 0x04090409; g_codePageText = "1252";           // en-US
    CheckInputLocale();
    bad |= g_pszIndicatior != s_A || g_uCodePage != 1252 || LeadByteTable[0x81] != 0;
    if (bad) { std::printf("00cb21a0_TEST FAIL\n"); return 1; }
    std::printf("00cb21a0_TEST PASS\n");
    return 0;
}