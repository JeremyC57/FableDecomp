#include <cstdio>
struct CSectionDesc { void* ShortName; void* LongName; bool Loaded; long Size; };
static CSectionDesc g_desc[4]; static int g_calls = 0;
class CCodeSectionManager { public:
    virtual void v00() { } unsigned char _maps[0x24]; bool SectionLoadingEnabled;
    CSectionDesc& GetSectionDesc(unsigned long h) { ++g_calls; return g_desc[h < 4 ? h : 3]; }
    void LoadSection(unsigned long handle) { if (handle != (unsigned long)-1) { if (!GetSectionDesc(handle).Loaded) GetSectionDesc(handle).Loaded = true; } }
};
int main()
{
    CCodeSectionManager m;
    m.LoadSection(1); if (!g_desc[1].Loaded || g_calls != 2) { std::printf("009f02c0_TEST FAIL\n"); return 1; }
    m.LoadSection(1); if (!g_desc[1].Loaded || g_calls != 3) { std::printf("009f02c0_TEST FAIL\n"); return 1; }
    m.LoadSection((unsigned long)-1); if (g_calls != 3 || g_desc[3].Loaded) { std::printf("009f02c0_TEST FAIL\n"); return 1; }
    std::printf("009f02c0_TEST PASS\n"); return 0;
}