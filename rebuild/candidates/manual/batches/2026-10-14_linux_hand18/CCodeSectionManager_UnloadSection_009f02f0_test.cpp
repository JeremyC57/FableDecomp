#include <cstdio>
struct CSectionDesc { void* ShortName; void* LongName; bool Loaded; long Size; };
static CSectionDesc g_desc[4]; static int g_calls = 0;
struct CCodeSectionManager {
    virtual void v00() { } unsigned char _maps[0x24]; bool SectionLoadingEnabled;
    CSectionDesc& GetSectionDesc(unsigned long h) { ++g_calls; return g_desc[h < 4 ? h : 3]; }
    void UnloadSection(unsigned long handle) { if (handle != (unsigned long)-1) { if (GetSectionDesc(handle).Loaded) GetSectionDesc(handle).Loaded = false; } }
};
int main()
{
    CCodeSectionManager m; g_desc[2].Loaded = true; g_desc[3].Loaded = true;
    m.UnloadSection(2); if (g_desc[2].Loaded || g_calls != 2) { std::printf("009f02f0_TEST FAIL\n"); return 1; }
    m.UnloadSection(2); if (g_desc[2].Loaded || g_calls != 3) { std::printf("009f02f0_TEST FAIL\n"); return 1; }
    m.UnloadSection((unsigned long)-1); if (g_calls != 3 || !g_desc[3].Loaded) { std::printf("009f02f0_TEST FAIL\n"); return 1; }
    std::printf("009f02f0_TEST PASS\n"); return 0;
}
