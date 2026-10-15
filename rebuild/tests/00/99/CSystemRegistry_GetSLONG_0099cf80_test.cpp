// Behaviour test for CSystemRegistry::GetSLONG @ 0x0099CF80: the stored DWORD on success, else (closed
// key or failed query) the default.
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

static long g_queryResult = 0;
static unsigned long g_queryValue = 0;
static long RegQueryValueExWStub(HKEY__* k, const unsigned short* n, unsigned long* reserved, unsigned long* type,
                                 unsigned char* data, unsigned long* size)
{
    ++g_calls; g_lastKey = k; g_lastName = n;
    if (reserved != 0 || *type != 0 || *size != 4) return 99;
    if (g_queryResult == 0) { std::memcpy(data, &g_queryValue, 4); *type = 4; }
    return g_queryResult;
}

class CSystemRegistry {
public:
    HKEY__* RootKey;
    HKEY__* OpenKey;
    long GetSLONG(const CWideString& valueName, long defaultValue) const
    {
        if (OpenKey != 0) {
            unsigned long type = 0;
            long value = 0;
            unsigned long size = 4;
            if (RegQueryValueExWStub(OpenKey, valueName, 0, &type, (unsigned char*)&value, &size) == 0)
                return value;
        }
        return defaultValue;
    }
};

int main()
{
    CWideString name(kName);
    CSystemRegistry r; r.RootKey = 0; r.OpenKey = &g_key;
    bool ok = true;
    g_queryResult = 0; g_queryValue = 1234;
    if (r.GetSLONG(name, -1) != 1234 || g_lastKey != &g_key || g_lastName != kName) ok = false;
    g_queryResult = 2;                           // ERROR_FILE_NOT_FOUND
    if (r.GetSLONG(name, -7) != -7) ok = false;
    r.OpenKey = 0;
    int before = g_calls;
    if (r.GetSLONG(name, 55) != 55 || g_calls != before) ok = false;
    (void)RegSetValueExWStub;
    if (!ok) { std::printf("0099cf80_TEST FAIL\n"); return 1; }
    std::printf("0099cf80_TEST PASS\n");
    return 0;
}