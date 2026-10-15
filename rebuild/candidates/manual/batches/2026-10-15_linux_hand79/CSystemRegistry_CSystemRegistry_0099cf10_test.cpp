// Behaviour test for CSystemRegistry::CSystemRegistry @ 0x0099CF10: selector 0 -> HKCU, 2 -> HKCR, anything
// else -> HKLM; the key is created with KEY_ALL_ACCESS and cleared when creation fails.
#include <cstdio>
#include <cstring>

struct HKEY__ { int unused; };

class CWideString {
public:
    explicit CWideString(const unsigned short* p) : m_pData(p) {}
    long GetLength() const { long n = 0; while (m_pData[n]) ++n; return n; }
    operator const unsigned short*() const { return m_pData; }
private:
    const unsigned short* m_pData;
};

static const unsigned short kName[] = { 0x56, 0x6f, 0x6c, 0 };   // "Vol"
static HKEY__ g_key;
static HKEY__* g_lastKey = 0;
static const unsigned short* g_lastName = 0;
static unsigned long g_lastType = 0xFFFF;
static const unsigned char* g_lastData = 0;
static unsigned long g_lastSize = 0;
static int g_storedDword = 0;
static int g_calls = 0;
static long RegSetValueExWStub(HKEY__* k, const unsigned short* n, unsigned long, unsigned long t,
                               const unsigned char* d, unsigned long s)
{
    ++g_calls; g_lastKey = k; g_lastName = n; g_lastType = t; g_lastData = d; g_lastSize = s;
    if (t == 4 && s == 4) std::memcpy(&g_storedDword, d, 4);
    return 0;
}

static long g_createResult = 0;
static unsigned long g_lastAccess = 0;
static HKEY__* g_lastRoot = 0;
static long RegCreateKeyExWStub(HKEY__* root, const unsigned short* sub, unsigned long, unsigned short*,
                                unsigned long, unsigned long access, void*, HKEY__** out, unsigned long*)
{
    ++g_calls; g_lastRoot = root; g_lastName = sub; g_lastAccess = access;
    *out = &g_key;
    return g_createResult;
}

class CSystemRegistry {
public:
    CSystemRegistry(const CWideString& subKey, long rootSelection)
    {
        switch (rootSelection) {
        case 0: RootKey = (HKEY__*)0x80000001; break;
        case 2: RootKey = (HKEY__*)0x80000000; break;
        default: RootKey = (HKEY__*)0x80000002; break;
        }
        if (RegCreateKeyExWStub(RootKey, subKey, 0, 0, 0, 0x2001F, 0, &OpenKey, 0) != 0)
            OpenKey = 0;
    }
    HKEY__* RootKey;
    HKEY__* OpenKey;
};

int main()
{
    CWideString sub(kName);
    bool ok = true;
    CSystemRegistry a(sub, 0);
    if (a.RootKey != (HKEY__*)0x80000001 || a.OpenKey != &g_key || g_lastAccess != 0x2001F || g_lastName != kName) ok = false;
    CSystemRegistry b(sub, 2);
    if (b.RootKey != (HKEY__*)0x80000000 || g_lastRoot != (HKEY__*)0x80000000) ok = false;
    CSystemRegistry c(sub, 1);
    if (c.RootKey != (HKEY__*)0x80000002) ok = false;
    g_createResult = 5;                          // ERROR_ACCESS_DENIED
    CSystemRegistry d(sub, 0);
    if (d.OpenKey != 0) ok = false;
    (void)RegSetValueExWStub;
    if (!ok) { std::printf("0099cf10_TEST FAIL\n"); return 1; }
    std::printf("0099cf10_TEST PASS\n");
    return 0;
}
