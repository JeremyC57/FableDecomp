// Behaviour test for ImeUi.cpp GetReadingWindowOrientation (0x00CB3420): standalone, Win32 registry calls faked.
#include <cstdio>
#include <cstring>
typedef unsigned int DWORD;
typedef unsigned char BYTE;
typedef int LONG;
typedef unsigned int HKL;
typedef int HKEY;
static HKL  g_hklCurrent = 0;
static bool g_bHorizontalReading = false;
static char g_path[300];
static int g_opens = 0, g_queries = 0, g_closes = 0;
static LONG g_openRc = 0, g_queryRc = 0;
static DWORD g_mapping = 0;
static char g_value[32];
#define HKEY_CURRENT_USER 0x80000001
#define KEY_READ 0x20019
static char* lstrcpyA(char* a, const char* b) { return std::strcpy(a, b); }
static char* lstrcatA(char* a, const char* b) { return std::strcat(a, b); }
static LONG RegOpenKeyExA(HKEY root, const char* sub, DWORD opt, DWORD sam, HKEY* out)
{
    g_opens++; std::strcpy(g_path, sub);
    if (root != (HKEY)HKEY_CURRENT_USER || opt != 0 || sam != KEY_READ) return 87;
    if (g_openRc) return g_openRc; *out = 42; return 0;
}
static LONG RegQueryValueExA(HKEY k, const char* name, DWORD* res, DWORD* type, BYTE* data, DWORD* cb)
{
    g_queries++; std::strcpy(g_value, name);
    if (k != 42 || res != 0 || *cb != 4) return 87;
    if (g_queryRc) return g_queryRc; *type = 4; std::memcpy(data, &g_mapping, 4); return 0;
}
static LONG RegCloseKey(HKEY k) { if (k == 42) g_closes++; return 0; }
#define MAX_PATH 260
#define ERROR_SUCCESS 0
#define LANG_CHT 0x0404
#define _CHT_HKL_NEW_CHANG_JIE 0xE0090404u
#define _CHS_HKL 0xE00E0804u
#define MAKEIMEVERSION(major, minor) ((DWORD)(((BYTE)(major) << 24) | ((BYTE)(minor) << 16)))
#define IMEID_VER(id)  ((id) & 0xffff0000)
#define IMEID_LANG(id) ((id) & 0x0000ffff)
static void GetReadingWindowOrientation(DWORD dwId)
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
                if ((dwVer <= MAKEIMEVERSION(5, 0) && ((BYTE)dwMapping == 0x22 || (BYTE)dwMapping == 0x23)) ||
                    ((dwVer == MAKEIMEVERSION(5, 1) || dwVer == MAKEIMEVERSION(5, 2)) &&
                     (BYTE)dwMapping >= 0x22 && (BYTE)dwMapping <= 0x24))
                {
                    g_bHorizontalReading = true;
                }
            }
            RegCloseKey(hkey);
        }
    }
}
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed at line %d\n", __LINE__); } } while (0)
static bool run(HKL hkl, DWORD id, DWORD mapping) { g_hklCurrent = hkl; g_mapping = mapping; g_bHorizontalReading = !g_bHorizontalReading; GetReadingWindowOrientation(id); return g_bHorizontalReading; }
int main()
{
    // MSPY, New Chang Jie and unknown ids are horizontal without touching the registry
    CHECK(run(_CHS_HKL, 0x05010404, 0) == true);
    CHECK(run(_CHT_HKL_NEW_CHANG_JIE, 0x05010404, 0) == true);
    CHECK(run(0x04040404, 0, 0) == true);
    CHECK(g_opens == 0);
    // non-CHT ids are vertical, no registry
    CHECK(run(0x04040404, 0x05010411, 0x22) == false); CHECK(g_opens == 0);
    // IME 5.0 and older: TINTLGNT, mappings 0x22/0x23 only
    CHECK(run(0x04040404, 0x05000404, 0x122) == true); CHECK(std::strcmp(g_path, "software\\microsoft\\windows\\currentversion\\TINTLGNT") == 0);
    CHECK(std::strcmp(g_value, "keyboard mapping") == 0);
    CHECK(run(0x04040404, 0x04000404, 0x23) == true);
    CHECK(run(0x04040404, 0x05000404, 0x24) == false);
    // IME 5.1 / 5.2: MSTCIPH, mappings 0x22..0x24
    CHECK(run(0x04040404, 0x05010404, 0x24) == true); CHECK(std::strcmp(g_path, "software\\microsoft\\windows\\currentversion\\MSTCIPH") == 0);
    CHECK(run(0x04040404, 0x05020404, 0x22) == true);
    CHECK(run(0x04040404, 0x05020404, 0x25) == false);
    CHECK(run(0x04040404, 0x05030404, 0x22) == false);   // 5.3: MSTCIPH but no rule
    CHECK(g_opens == 7 && g_queries == 7 && g_closes == 7);
    // registry failures leave it vertical; the key is closed only when it was opened
    g_queryRc = 2; CHECK(run(0x04040404, 0x05010404, 0x22) == false); CHECK(g_closes == 8); g_queryRc = 0;
    g_openRc = 2;  CHECK(run(0x04040404, 0x05010404, 0x22) == false); CHECK(g_closes == 8 && g_queries == 8 && g_opens == 9); g_openRc = 0;
    if (g_fail) { std::printf("00cb3420_TEST FAIL\n"); return 1; }
    std::printf("00cb3420_TEST PASS\n");
    return 0;
}