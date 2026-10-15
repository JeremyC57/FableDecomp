// Behaviour test for CSystemRegistry::WriteSLONG @ 0x0099D0C0: a REG_DWORD of the value under the open
// key; nothing when the key is closed.
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

class CSystemRegistry {
public:
    HKEY__* RootKey;
    HKEY__* OpenKey;
    void WriteSLONG(const CWideString& valueName, long value) const
    {
        if (OpenKey != 0) {
            long stored = value;
            RegSetValueExWStub(OpenKey, valueName, 0, 4, (const unsigned char*)&stored, 4);
        }
    }
};

int main()
{
    CWideString name(kName);
    CSystemRegistry r; r.RootKey = 0; r.OpenKey = &g_key;
    bool ok = true;
    r.WriteSLONG(name, -12345);
    if (g_calls != 1 || g_lastKey != &g_key || g_lastName != kName || g_lastType != 4 || g_lastSize != 4
        || g_storedDword != -12345) ok = false;
    r.OpenKey = 0;
    r.WriteSLONG(name, 7);
    if (g_calls != 1) ok = false;
    if (!ok) { std::printf("0099d0c0_TEST FAIL\n"); return 1; }
    std::printf("0099d0c0_TEST PASS\n");
    return 0;
}
