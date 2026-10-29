// Behaviour test for CSystemRegistry::GetString @ 0x0099D000: a successful REG_SZ query returns a new string built
// from the 512-character buffer (query passed reserved 0, type 0, size 0x200); a failed query or a closed key returns a
// copy of the default (copy ctor), without querying when the key is closed.
#include <cstdio>
#include <cstring>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)

struct HKEY__ { int unused; };

static int g_textCtors = 0, g_copyCtors = 0;
class CWideString {
public:
    CWideString(const unsigned short* text) : m_pData(dup(text)) { ++g_textCtors; }       // 0x0099B6B0
    CWideString(const CWideString& other) : m_pData(dup(other.m_pData)) { ++g_copyCtors; } // 0x0099B720
    ~CWideString() { delete[] m_pData; }
    const unsigned short* GetAsCharArray() const { return m_pData; }                             // 0x0099AF10
private:
    static unsigned short* dup(const unsigned short* s)
    { long n = 0; while (s[n]) ++n; unsigned short* p = new unsigned short[n + 1]; std::memcpy(p, s, (n + 1) * 2); return p; }
    CWideString& operator=(const CWideString&);
    unsigned short* m_pData;
};

static bool same(const unsigned short* a, const unsigned short* b)
{ while (*a && *a == *b) { ++a; ++b; } return *a == *b; }

static const unsigned short kName[] = { 'N', 'a', 'm', 'e', 0 };
static const unsigned short kDef[]  = { 'd', 'e', 'f', 0 };
static const unsigned short kVal[]  = { 'H', 'e', 'r', 'o', '!', 0 };
static HKEY__ g_key;
static int g_calls = 0;
static long g_result = 0;
static HKEY__* g_lastKey = 0;
static const unsigned short* g_lastName = 0;
static unsigned long g_lastSize = 0;
static long RegQueryValueExW(HKEY__* hKey, const unsigned short* lpValueName, unsigned long* lpReserved,
                             unsigned long* lpType, unsigned char* lpData, unsigned long* lpcbData)
{
    ++g_calls; g_lastKey = hKey; g_lastName = lpValueName; g_lastSize = *lpcbData;
    if (lpReserved != 0 || *lpType != 0 || *lpcbData != 0x200) return 87;
    if (g_result == 0) { std::memcpy(lpData, kVal, sizeof(kVal)); *lpType = 1; *lpcbData = sizeof(kVal); }
    else std::memcpy(lpData, kDef, 2);                 // garbage the failure path must not use
    return g_result;
}

class CSystemRegistry {
public:
    CWideString GetString(const CWideString& valueName, const CWideString& defaultValue) const;
    HKEY__* RootKey;                             // +0x00
    HKEY__* OpenKey;                             // +0x04
};

CWideString CSystemRegistry::GetString(const CWideString& valueName, const CWideString& defaultValue) const
{
    if (OpenKey != 0) {
        unsigned short buffer[512];
        unsigned long type = 0;
        unsigned long size = 0x200;
        if (RegQueryValueExW(OpenKey, valueName.GetAsCharArray(), 0, &type, (unsigned char*)buffer, &size) == 0)
            return CWideString(buffer);
    }
    return CWideString(defaultValue);
}

int main()
{
    CWideString name(kName), def(kDef);
    CSystemRegistry r; r.RootKey = 0; r.OpenKey = &g_key;

    g_textCtors = g_copyCtors = 0; g_result = 0;
    {
        CWideString s = r.GetString(name, def);
        CHECK(same(s.GetAsCharArray(), kVal) && g_calls == 1 && g_lastKey == &g_key && same(g_lastName, kName) && g_lastSize == 0x200);
        CHECK(g_textCtors == 1);
        CHECK(s.GetAsCharArray() != def.GetAsCharArray());
    }

    g_textCtors = g_copyCtors = 0; g_result = 2;      // ERROR_FILE_NOT_FOUND
    {
        CWideString s = r.GetString(name, def);
        CHECK(same(s.GetAsCharArray(), kDef) && g_calls == 2 && g_textCtors == 0 && g_copyCtors >= 1);
        CHECK(s.GetAsCharArray() != def.GetAsCharArray());
    }

    r.OpenKey = 0; g_textCtors = g_copyCtors = 0; g_result = 0;
    {
        CWideString s = r.GetString(name, def);
        CHECK(same(s.GetAsCharArray(), kDef) && g_calls == 2 && g_textCtors == 0 && g_copyCtors >= 1);
    }

    if (g_fail) { std::printf("0099d000_TEST FAIL\n"); return 1; }
    std::printf("0099d000_TEST PASS\n");
    return 0;
}