// Behaviour test for CRegistryKey::QueryDWord 0x00C73830: RegQueryValueExA is called on the key's handle with the name, a null
// reserved pointer, the caller's buffer and a DWORD-sized size; the result is 1 only when the query succeeds, the type is
// REG_DWORD (4) and four bytes came back, else 0. RegQueryValueExA is a test stub.
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __stdcall
#endif
#define RegQueryValueExA TestRegQueryValueExA
typedef struct HKEY__* HKEY;
extern "C" long __stdcall RegQueryValueExA(HKEY key, const char* name, unsigned long* reserved,
    unsigned long* type, unsigned char* data, unsigned long* size);

struct CRegistryKey {
    virtual void Dummy();                  // vtable 0x012BEFB0
    HKEY m_hKey;                           // +0x04
    int QueryDWord(const char* name, unsigned long* value);
};

int CRegistryKey::QueryDWord(const char* name, unsigned long* value)
{
    unsigned long type;
    unsigned long size = sizeof(unsigned long);
    if (RegQueryValueExA(m_hKey, name, 0, &type, (unsigned char*)value, &size) != 0)
        return 0;
    else if (type != 4)
        return 0;
    else
        return size == sizeof(unsigned long);
}
void CRegistryKey::Dummy() { }

static long g_result; static unsigned long g_type, g_size, g_value; static int g_calls;
static HKEY g_key; static const char* g_name; static unsigned long* g_reserved; static unsigned long g_sizeIn;
extern "C" long __stdcall TestRegQueryValueExA(HKEY key, const char* name, unsigned long* reserved,
    unsigned long* type, unsigned char* data, unsigned long* size)
{
    ++g_calls; g_key = key; g_name = name; g_reserved = reserved; g_sizeIn = *size;
    if (g_result == 0) { *type = g_type; std::memcpy(data, &g_value, sizeof(g_value)); *size = g_size; }
    return g_result;
}
static int fail(int n) { std::printf("00c73830_TEST FAIL %d\n", n); return 1; }
int main()
{
    static int keyObj;
    CRegistryKey key; key.m_hKey = reinterpret_cast<HKEY>(&keyObj);
    unsigned long out = 0;
    g_result = 0; g_type = 4; g_size = sizeof(unsigned long); g_value = 0x12345678;
    if (key.QueryDWord("Flags", &out) != 1 || out != 0x12345678) return fail(1);
    if (g_calls != 1 || g_key != key.m_hKey || std::strcmp(g_name, "Flags") != 0 || g_reserved != 0 || g_sizeIn != sizeof(unsigned long)) return fail(2);
    g_type = 1;                                        // REG_SZ
    if (key.QueryDWord("Flags", &out) != 0) return fail(3);
    g_type = 4; g_size = 2;                            // short read
    if (key.QueryDWord("Flags", &out) != 0) return fail(4);
    g_result = 2; g_size = sizeof(unsigned long);                          // ERROR_FILE_NOT_FOUND
    if (key.QueryDWord("Missing", &out) != 0 || g_calls != 4) return fail(5);
    std::printf("00c73830_TEST PASS\n");
    return 0;
}