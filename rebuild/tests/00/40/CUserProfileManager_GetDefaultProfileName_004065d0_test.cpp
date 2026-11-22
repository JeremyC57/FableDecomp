// Test for 0x004065D0 CUserProfileManager::GetDefaultProfileName() const on the shared headers (NF236): the row's defines, includes and
// body; the CWideString text constructor (retail 0x0099B6B0) and destructor are stubbed to record their argument. Case 1: one call builds
// the result from the wide literal "Default" (seven characters, then the terminator) exactly once. Case 2: a second call (const object)
// passes the same text again and does not touch the manager object.
#include <cstdio>
#include <cstring>
#define FABLE_RETAIL_CWIDESTRING
#include "fable_string.h"
#define FABLE_RETAIL_CUSERPROFILEMANAGER_DEFAULTPROFILENAME
#include "engine/CUserProfileManager.h"
static const wchar_t* g_seen = 0;
static int g_ctors = 0;
CWideString::CWideString(const wchar_t* text) : PStringData(0) { g_seen = text; ++g_ctors; }
CWideString::~CWideString() {}
long CWideString::NoStrings = 0;
CWideString CUserProfileManager::GetDefaultProfileName() const
{
    return L"Default";
}
static bool IsDefault(const wchar_t* s)
{
    const char* want = "Default";
    if (s == 0) return false;
    for (int i = 0; i < 8; ++i) if (s[i] != (wchar_t)(unsigned char)want[i]) return false;
    return true;
}
int main()
{
    int fails = 0;
    static unsigned char raw[sizeof(CUserProfileManager)];
    static unsigned char want[sizeof(CUserProfileManager)];
    std::memset(raw, 0x5A, sizeof(raw)); std::memset(want, 0x5A, sizeof(want));
    const CUserProfileManager& m = *(const CUserProfileManager*)raw;
    {
        CWideString r = m.GetDefaultProfileName();
        if (g_ctors != 1 || !IsDefault(g_seen)) fails |= 1;
    }
    const wchar_t* first = g_seen;
    {
        CWideString r = m.GetDefaultProfileName();
        if (g_ctors != 2 || g_seen != first || !IsDefault(g_seen)) fails |= 2;
    }
    if (std::memcmp(raw, want, sizeof(raw)) != 0) fails |= 4;
    if (fails) { std::printf("004065d0_TEST FAIL %d\n", fails); return 1; }
    std::printf("004065d0_TEST PASS\n");
    return 0;
}
