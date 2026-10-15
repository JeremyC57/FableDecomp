// Behaviour test for the CRegisteredTCInfo constructor @ 0x004D2EF0: allocator, driver type and a copied name.
#include <cstdio>
#include <cstring>

struct CThing; struct CTCBase;
static int g_copies = 0;
struct CCharString {
    char m_text[32];
    CCharString(const char* s) { std::strcpy(m_text, s); }
    CCharString(const CCharString& other) { ++g_copies; std::strcpy(m_text, other.m_text); }
};
struct CRegisteredTCInfo {
    typedef CTCBase* (*TAllocFunc)(CThing*);
    TAllocFunc AllocFunc; unsigned long DriverType; CCharString Name;
    CRegisteredTCInfo(TAllocFunc allocFunc, unsigned long driverType, const CCharString& name)
        : AllocFunc(allocFunc), DriverType(driverType), Name(name) {}
};
static CTCBase* AllocLight(CThing*) { return 0; }

int main()
{
    CCharString name("TC_LIGHT");
    CRegisteredTCInfo info(AllocLight, 42, name);
    if (info.AllocFunc != AllocLight || info.DriverType != 42 || g_copies != 1 || std::strcmp(info.Name.m_text, "TC_LIGHT") != 0) {
        std::printf("004d2ef0_TEST FAIL\n"); return 1;
    }
    std::printf("004d2ef0_TEST PASS\n");
    return 0;
}