// Behaviour test for CSystemRegistry::DeleteValue @ 0x0099D1E0: RegDeleteValueW on the open key with
// the name, with no open-key test.
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

static int g_deletes = 0;
static long RegDeleteValueWStub(HKEY__* k, const unsigned short* n) { ++g_deletes; g_lastKey = k; g_lastName = n; return 0; }

class CSystemRegistry {
public:
    HKEY__* RootKey;
    HKEY__* OpenKey;
    void DeleteValue(const CWideString& valueName) const { RegDeleteValueWStub(OpenKey, valueName); }
};

int main()
{
    CWideString name(kName);
    CSystemRegistry r; r.RootKey = 0; r.OpenKey = &g_key;
    bool ok = true;
    r.DeleteValue(name);
    if (g_deletes != 1 || g_lastKey != &g_key || g_lastName != kName) ok = false;
    r.OpenKey = 0;
    r.DeleteValue(name);                         // still called, with a null key
    if (g_deletes != 2 || g_lastKey != 0) ok = false;
    (void)g_calls; (void)RegSetValueExWStub;
    if (!ok) { std::printf("0099d1e0_TEST FAIL\n"); return 1; }
    std::printf("0099d1e0_TEST PASS\n");
    return 0;
}