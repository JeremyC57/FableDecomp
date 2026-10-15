// Behaviour test for CSystemRegistry::WriteBinary @ 0x0099D180: REG_BINARY with the caller's block and
// size; nothing when the key is closed.
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
    void WriteBinary(const CWideString& valueName, void* data, unsigned long size) const
    {
        if (OpenKey != 0)
            RegSetValueExWStub(OpenKey, valueName, 0, 3, (const unsigned char*)data, size);
    }
};

int main()
{
    unsigned char block[10];
    CWideString name(kName);
    CSystemRegistry r; r.RootKey = 0; r.OpenKey = &g_key;
    bool ok = true;
    r.WriteBinary(name, block, sizeof(block));
    if (g_calls != 1 || g_lastKey != &g_key || g_lastName != kName || g_lastType != 3
        || g_lastData != block || g_lastSize != 10) ok = false;
    r.OpenKey = 0;
    r.WriteBinary(name, block, 3);
    if (g_calls != 1) ok = false;
    if (!ok) { std::printf("0099d180_TEST FAIL\n"); return 1; }
    std::printf("0099d180_TEST PASS\n");
    return 0;
}
