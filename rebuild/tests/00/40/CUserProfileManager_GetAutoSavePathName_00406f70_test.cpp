// Behaviour test for 0x00406F70 CUserProfileManager::GetAutoSavePathName: links the row object (the method is declared here, not
// defined) and asserts the texts the row passes: "U:\\" to CCharString (both paths), L"AutoSave" as the suffix on success, L"" on failure.
#include <cstdio>
typedef unsigned short wchar16;

// NF413: the row spells retail's literals, so the test compares the text it passes, not a retail address.
static bool RetailTextEquals(const void* p, const char* want)
{
    const char* s = (const char*)p;
    if (s == 0) return false;
    while (*want != 0) { if (*s != *want) return false; ++s; ++want; }
    return *s == 0;
}
static bool RetailWideTextEquals(const void* p, const char* want)   // ASCII text, compared unit by unit
{
    const wchar16* s = (const wchar16*)p;
    if (s == 0) return false;
    while (*want != 0) { if (*s != (wchar16)(unsigned char)*want) return false; ++s; ++want; }
    return *s == 0;
}

class CCharString { public: void* p; CCharString(const char*, long); ~CCharString(); };
class CWideString { public:
    void* p;
    CWideString(const wchar16* s);
    ~CWideString();
};

CWideString __fastcall WConcat(const wchar16* pre, const CWideString& suf);
extern "C" long __fastcall CreateSaveGame(CCharString*, CWideString*, long, long, wchar16*, long);

class CUserProfileManager { public:
    char pad[8];
    CWideString field8;
    CWideString GetAutoSavePathName();
};

// fake defs so it links
static const char* g_cs_text = 0;
static long g_cs_len = 0;
CCharString::CCharString(const char* s, long len) { p = 0; g_cs_text = s; g_cs_len = len; }
CCharString::~CCharString() {}
static const wchar16* g_last_pre = 0;
static const wchar16* g_last_single = 0;
static const void* g_last_suffix = 0;
CWideString::CWideString(const wchar16* s) { p = (void*)s; g_last_single = s; }
CWideString::~CWideString() {}
CWideString __fastcall WConcat(const wchar16* pre, const CWideString& suf) { g_last_pre = pre; g_last_suffix = suf.p; return CWideString((const wchar16*)0); }

static long g_ret = 0;
extern "C" long __fastcall CreateSaveGame(CCharString*, CWideString*, long, long, wchar16*, long) { return g_ret; }

int main() {
    char storage[64] = {0};
    CUserProfileManager& m = *reinterpret_cast<CUserProfileManager*>(storage);
    g_ret = 0;
    g_last_pre = 0; g_last_single = 0; g_last_suffix = 0; g_cs_text = 0; g_cs_len = 0;
    { CWideString r = m.GetAutoSavePathName(); (void)r; }
    bool ok_path = (g_last_pre != 0) && RetailWideTextEquals(g_last_suffix, "AutoSave")
                && RetailTextEquals(g_cs_text, "U:\\") && g_cs_len == -1;

    g_ret = 1;
    g_last_pre = 0; g_last_single = 0; g_last_suffix = 0; g_cs_text = 0; g_cs_len = 0;
    { CWideString r = m.GetAutoSavePathName(); (void)r; }
    bool ok_fail = (g_last_pre == 0) && RetailWideTextEquals(g_last_single, "")
                && RetailTextEquals(g_cs_text, "U:\\") && g_cs_len == -1;

    if (ok_path && ok_fail) printf("00406f70_TEST PASS\n");
    else { printf("FAIL path=%d fail=%d cs=%s\n", (int)ok_path, (int)ok_fail, g_cs_text ? g_cs_text : "(null)"); return 1; }
    return 0;
}
