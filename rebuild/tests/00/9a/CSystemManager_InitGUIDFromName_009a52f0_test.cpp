// Behaviour test for CSystemManager::InitGUIDFromName @ 0x009A52F0: the first 16 bytes of the name go
// into ApplicationGUID, zero-padded when the name is shorter.
#include <cstdio>
#include <cstring>

struct _GUID {
    unsigned long  Data1;
    unsigned short Data2;
    unsigned short Data3;
    unsigned char  Data4[8];
};

class CSystemManager { public:
    bool  ApplicationActive;
    _GUID ApplicationGUID;
    _GUID BestGUID;

    void InitGUIDFromName(const char* name)
    {
        char* guid = (char*)&ApplicationGUID;
        long length = (long)std::strlen(name);
        for (long i = 0; i < 16; ++i)
            guid[i] = (i < length) ? name[i] : '\0';
    }
};

int main()
{
    CSystemManager m;
    std::memset(&m, 0x55, sizeof(m));
    m.InitGUIDFromName("Fable");
    const unsigned char* g = (const unsigned char*)&m.ApplicationGUID;
    bool ok = std::memcmp(g, "Fable", 5) == 0;
    for (int i = 5; i < 16; ++i)
        if (g[i] != 0) ok = false;
    const unsigned char* b = (const unsigned char*)&m.BestGUID;
    if (b[0] != 0x55) ok = false;                 // never writes past the 16 bytes

    m.InitGUIDFromName("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
    if (std::memcmp(g, "ABCDEFGHIJKLMNOP", 16) != 0) ok = false;
    if (b[0] != 0x55) ok = false;

    m.InitGUIDFromName("");
    for (int i = 0; i < 16; ++i)
        if (g[i] != 0) ok = false;

    if (!ok) { std::printf("009a52f0_TEST FAIL\n"); return 1; }
    std::printf("009a52f0_TEST PASS\n");
    return 0;
}