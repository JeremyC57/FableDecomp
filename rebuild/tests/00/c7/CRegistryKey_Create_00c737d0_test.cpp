#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __cdecl
#define __stdcall
#define __fastcall
#define __declspec(x)
typedef long long __int64;
#endif
#define RegCreateKeyExA TestRegCreateKeyExA
typedef struct HKEY__* HKEY;
extern "C" long __stdcall RegCreateKeyExA(HKEY key, const char* subKey, unsigned long reserved,
    char* className, unsigned long options, unsigned long access, void* security, HKEY* result, unsigned long* disposition);

struct CRegistryKey {
    virtual void Dummy();                  // vtable 0x012BEFB0
    HKEY m_hKey;                           // +0x04
    int Create(HKEY parent, const char* subKey, unsigned long access);
};

int CRegistryKey::Create(HKEY parent, const char* subKey, unsigned long access)
{
    return RegCreateKeyExA(parent, subKey, 0, "", 0, access, 0, &m_hKey, 0) == 0;
}
static int fail() { std::printf("00c737d0_TEST FAIL\n"); return 1; }
static bool nearly(float a, float b) { float d = a - b; if (d < 0) d = -d; return d < 1e-3f; }
static long g_result = 0; static HKEY g_parent; static const char* g_sub; static unsigned long g_reserved, g_options, g_access;
static char* g_class; static void* g_security; static unsigned long* g_disposition; static int g_calls = 0; static int g_key;
extern "C" long __stdcall TestRegCreateKeyExA(HKEY key, const char* subKey, unsigned long reserved, char* className,
    unsigned long options, unsigned long access, void* security, HKEY* result, unsigned long* disposition)
{
    ++g_calls; g_parent = key; g_sub = subKey; g_reserved = reserved; g_class = className; g_options = options;
    g_access = access; g_security = security; g_disposition = disposition;
    if (g_result == 0) *result = reinterpret_cast<HKEY>(&g_key);
    return g_result;
}
void CRegistryKey::Dummy() { }
int main()
{
    static int parentKey;
    HKEY parent = reinterpret_cast<HKEY>(&parentKey);
    CRegistryKey key; key.m_hKey = 0;
    if (key.Create(parent, "Software\\Test", 0xF003F) != 1) return fail();
    if (g_calls != 1 || g_parent != parent || std::strcmp(g_sub, "Software\\Test") != 0 || g_reserved != 0) return fail();
    if (g_class == 0 || g_class[0] != 0 || g_options != 0 || g_access != 0xF003F || g_security != 0 || g_disposition != 0) return fail();
    if (key.m_hKey != reinterpret_cast<HKEY>(&g_key)) return fail();
    g_result = 5;                                             // ERROR_ACCESS_DENIED
    if (key.Create(parent, "x", 1) != 0 || g_calls != 2) return fail();
    std::printf("00c737d0_TEST PASS\n");
    return 0;
}