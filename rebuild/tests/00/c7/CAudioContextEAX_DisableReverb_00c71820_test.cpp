// Behaviour test for 0x00C71820 (standalone copy of the model and logic)
#include <cstdio>
#include <cstring>

typedef int (*PFN_EAXSet)(const void* guid, unsigned int property, unsigned int source, void* value, unsigned int size);
static unsigned int g_prop = 0, g_src = 99, g_size = 0; static int g_value = 0; static int g_sets = 0; static int g_resolves = 0;
static const unsigned char g_guid[16] = { 1 };
static int FakeEAXSet(const void* guid, unsigned int property, unsigned int source, void* value, unsigned int size)
{ g_sets++; g_prop = property; g_src = source; g_size = size; g_value = *(int*)value; return guid == g_guid ? 0 : 1; }
struct CAudioContext { PFN_EAXSet pfnEAXSet; void* GetProcAddress(const char* name) { g_resolves++; return std::strcmp(name, "EAXSet") == 0 ? (void*)FakeEAXSet : 0; } };
CAudioContext g_AudioContext;
struct X { bool m_enabled;
    void DisableReverb()
    {
        int room = -10000;
        if (!g_AudioContext.pfnEAXSet)
            g_AudioContext.pfnEAXSet = (PFN_EAXSet)g_AudioContext.GetProcAddress("EAXSet");
        g_AudioContext.pfnEAXSet(g_guid, 5, 0, &room, 4);
        m_enabled = false;
    } };
int main()
{
    g_AudioContext.pfnEAXSet = 0;
    X x; x.m_enabled = true;
    x.DisableReverb();
    if (x.m_enabled || g_resolves != 1 || g_sets != 1 || g_prop != 5 || g_src != 0 || g_size != 4 || g_value != -10000) { std::printf("00c71820_TEST FAIL\n"); return 1; }
    x.DisableReverb();
    if (g_resolves != 1 || g_sets != 2) { std::printf("00c71820_TEST FAIL\n"); return 1; }
    std::printf("00c71820_TEST PASS\n");
    return 0;
}