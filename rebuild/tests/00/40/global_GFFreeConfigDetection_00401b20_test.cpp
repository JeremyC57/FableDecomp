#include <cstdio>
#include <cstring>

// Model of 0x00401B20 GFFreeConfigDetection: call the optional "Config_Release" export of the detection DLL,
// FreeLibrary the module, clear the global handle, return false (stubs stand in for the Win32 imports).
typedef void* HMODULE;
typedef void (*PFNConfigRelease)();
static HMODULE g_hConfigDetectionModule = 0;
static int g_released = 0;
static int g_freed = 0;
static HMODULE g_freedModule = 0;
static bool g_hasExport = true;
static void ConfigRelease() { ++g_released; }
static PFNConfigRelease StubGetProcAddress(HMODULE, const char* name)
{
    return (g_hasExport && std::strcmp(name, "Config_Release") == 0) ? ConfigRelease : 0;
}
static int StubFreeLibrary(HMODULE module) { ++g_freed; g_freedModule = module; return 1; }

static bool GFFreeConfigDetection()
{
    PFNConfigRelease release = StubGetProcAddress(g_hConfigDetectionModule, "Config_Release");
    if (release)
        release();
    StubFreeLibrary(g_hConfigDetectionModule);
    g_hConfigDetectionModule = 0;
    return false;
}

int main()
{
    static int moduleImage;
    g_hConfigDetectionModule = &moduleImage;
    bool r1 = GFFreeConfigDetection();
    if (r1 || g_released != 1 || g_freed != 1 || g_freedModule != &moduleImage || g_hConfigDetectionModule != 0) {
        std::printf("00401b20_TEST FAIL\n");
        return 1;
    }
    g_hasExport = false;
    g_hConfigDetectionModule = &moduleImage;
    bool r2 = GFFreeConfigDetection();
    if (r2 || g_released != 1 || g_freed != 2 || g_hConfigDetectionModule != 0) {
        std::printf("00401b20_TEST FAIL\n");
        return 1;
    }
    std::printf("00401b20_TEST PASS\n");
    return 0;
}